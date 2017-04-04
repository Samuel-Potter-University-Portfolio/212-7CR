#include "MeshCollider.h"
#include "SphereCollider.h"
#include "Body.h"
#include "PhysicsScene.h"
#include <glm.hpp>


#define CLAMP(x, min, max) ( x > max ? max : x < min ? min : x)

void MeshCollider::SetMesh(Mesh* mesh) 
{
	this->mesh = mesh;
	bounding_radius = 0.0f;

	if (!mesh)
		return;

	const std::vector<float>& vertices = mesh->GetRawVertices();
	for (int i = 0; i < vertices.size(); i += 3) 
	{
		float sqrd_magnitude = vertices[i] * vertices[i] + vertices[i + 1] * vertices[i + 1] + vertices[i + 2] * vertices[i + 2];
		if (sqrd_magnitude > bounding_radius)
			bounding_radius = sqrd_magnitude;
	}

	if (bounding_radius != 0.0f)
		bounding_radius = sqrtf(bounding_radius);
}

void MeshCollider::ResolveCollision(Body* body, HitInfo& hit_info)
{
	Collider* collider = body->GetCollider();

	//Resolve Sphere
	{
		SphereCollider* sphere = Cast<SphereCollider>(collider);

		if (sphere)
		{
			DefaultResolution(body, hit_info);
			//body->velocity *= 0.95f;
			return;

			const glm::vec3 correction = hit_info.normal * -hit_info.embedded_distance;
			const glm::vec3 start_velocity = body->velocity;

			body->velocity += correction;
			body->ApplyFriction(properties.friction);

			//glm::vec3 reflection = glm::reflect(hit_info.normal, start_velocity);
			//body->ApplyForce(body->mass * -correction);
			//body->velocity += reflection * 0.0f;
			//body->ApplyForce(0.1f * body->mass * correction);
			//body->angular_velocity += xyz;


			//Call appropriate functions
			GameObject* body_parent = Cast<GameObject>(body->GetParent());
			if (body_parent)
				body_parent->OnCollide(this, hit_info);
			if (game_object)
			{
				HitInfo other_hit = hit_info;
				other_hit.normal *= -1.0f;
				game_object->OnCollide(body->GetCollider(), other_hit);
			}

			//Apply appropriate force
			if (attached_body)
				attached_body->ApplyForce(body->mass * -correction);


			return;
		}
	}
}

bool MeshCollider::DoesNarrowPhaseCollide(Collider* collider, glm::vec3 velocity, HitInfo& hit_info)
{
	//Mesh on Sphere
	{
		SphereCollider* sphere = Cast<SphereCollider>(collider);

		if (sphere && DoesSphereCollide(sphere, velocity, hit_info))
			return true;
	}

	return false;
}

bool MeshCollider::DoesSphereCollide(SphereCollider* sphere, glm::vec3 velocity, HitInfo& hit_info)
{
	if (!mesh || sphere->radius == 0.0f)
		return false;

	const float& radius = sphere->radius;
	const glm::vec3 sphere_location = sphere->GetWorldLocation() / radius;
	velocity /= radius;

	const glm::vec3 world_location = GetWorldLocation() / radius;
	const glm::vec3 world_scale = GetWorldScale() / radius;
	const std::vector<int>& indices = mesh->GetRawIndices();
	const std::vector<float>& vertices = mesh->GetRawVertices();

	TriangleHit nearest_hit;
	bool has_hit = false;

	for (int i = 0; i < indices.size(); i += 3) 
	{
		const int& tri_i0 = indices[i];
		const int& tri_i1 = indices[i + 1];
		const int& tri_i2 = indices[i + 2];

		glm::vec3 tri_p0(
			vertices[tri_i0 * 3],
			vertices[tri_i0 * 3 + 1],
			vertices[tri_i0 * 3 + 2]
		);

		glm::vec3 tri_p1(
			vertices[tri_i1 * 3],
			vertices[tri_i1 * 3 + 1],
			vertices[tri_i1 * 3 + 2]
		);

		glm::vec3 tri_p2(
			vertices[tri_i2 * 3],
			vertices[tri_i2 * 3 + 1],
			vertices[tri_i2 * 3 + 2]
		);

		tri_p0 *= world_scale;
		tri_p1 *= world_scale;
		tri_p2 *= world_scale;

		tri_p0 += world_location;
		tri_p1 += world_location;
		tri_p2 += world_location;

		//Check triangle against unit sphere
		Triangle triangle(tri_p0, tri_p1, tri_p2);
		TriangleHit triangle_hit;
		
		if (DoesUnitSphereCollide(sphere_location, velocity, triangle, triangle_hit))
		{
			if (!has_hit || triangle_hit.distance < nearest_hit.distance)
			{
				nearest_hit = triangle_hit;
				has_hit = true;
			}
		}
	}

	if (!has_hit)
		return false;
	
	const glm::vec3 location_at_hit = sphere_location + velocity * nearest_hit.time;
	const glm::vec3 hit_normal = glm::normalize(location_at_hit - nearest_hit.hit_location);

	//Slide on plane where hit_normal is plane normal
	const float plane_d = -glm::dot(hit_normal, nearest_hit.hit_location);
	const float plane_distance = glm::dot(hit_normal, sphere_location + velocity) + plane_d - 1.0f;
	
	//Correct hit_info (Exit sphere space)
	hit_info.embedded_distance = plane_distance * radius;
	hit_info.hit_location = location_at_hit;
	hit_info.normal = hit_normal;
	
	if (hit_info.embedded_distance >= 0.0f)
		return false;

	return true;
}

/*
Finds the lowest root for Ax^2 + Bx + C = 0 that is in range [min_root, max_root]
x = (-B +- sqrt(B^2 - 4AC))/2A
*/
bool HasLowestRoot(float A, float B, float C, float max_root, float& out)
{
	const float determinant = B*B - 4.0f * A * C;

	if (determinant < 0.0f)
		return false;

	const float sqrt_det = sqrtf(determinant);
	float root_0 = (-B - sqrt_det) / (2.0f * A);
	float root_1 = (-B + sqrt_det) / (2.0f * A);

	//Put roots in size order
	if (root_0 > root_1)
	{
		float temp = root_0;
		root_0 = root_1;
		root_1 = temp;
	}

	if (root_0 > 0.0f && root_0 < max_root)
	{
		out = root_0;
		return true;
	}
	if (root_1 > 0.0f && root_1 < max_root)
	{
		out = root_1;
		return true;
	}

	return false;
}

bool MeshCollider::DoesUnitSphereCollide(glm::vec3 location, glm::vec3 velocity, Triangle& triangle, TriangleHit& triangle_hit)
{
	if (!triangle.IsFacing(glm::normalize(velocity)))
		return false;

	//Check against the triangle plane
	float t0, t1;
	bool embedded = false;

	float distance = triangle.PlaneDistance(location);
	float tri_normal_dot_velocity = glm::dot(triangle.GetNormal(), velocity);

	//Travelling parallel to the triangle
	if (tri_normal_dot_velocity == 0.0f)
	{
		//No collision 
		if (fabs(distance) >= 1.0f)
			return false;

		else
		{
			embedded = true;
			t0 = 0.0f;
			t1 = 1.0f;
		}
	}
	else 
	{
		//Calculate intersection times with plane
		t0 = (-1.0f - distance) / tri_normal_dot_velocity;
		t1 = (1.0f - distance) / tri_normal_dot_velocity;
	
		//Make sure in order
		if (t0 > t1)
		{
			float temp = t0;
			t0 = t1;
			t1 = temp;
		}

		//Check that at least 1 is in desired time range 0 to 1
		if (t0 > 1.0f || t1 < 0.0f)
			return false;

		t0 = CLAMP(t0, 0.0f, 1.0f);
		t1 = CLAMP(t1, 0.0f, 1.0f);
	}


	//Actual collision info
	bool collision_found = false;
	float t = 1.0f;
	glm::vec3 collision_point;


	//Check against plane
	if (!embedded)
	{
		glm::vec3 plane_intersection = location + t0 * velocity - triangle.GetNormal();

		if (triangle.IsInside(plane_intersection))
		{
			collision_found = true;
			t = t0;
			collision_point = plane_intersection;
		}
	}

	//More complex checks
	if (!collision_found && !embedded)
	{
		float velocity_length_sqrd = glm::dot(velocity, velocity);

		//VERTS
		//A
		{
			float root;
			const glm::vec3& point = triangle.GetVertA();
			const glm::vec3 to_point = point - location;

			if (
				HasLowestRoot(
					velocity_length_sqrd,
					2.0f * (glm::dot(velocity, location - point)),
					glm::dot(to_point, to_point) - 1.0f,
					t,
					root
				)
			)
			{
				t = root;
				collision_found = true;
				collision_point = point;
			}
		}

		//B
		{
			float root;
			const glm::vec3& point = triangle.GetVertB();
			const glm::vec3 to_point = point - location;

			if (
				HasLowestRoot(
					velocity_length_sqrd,
					2.0f * (glm::dot(velocity, location - point)),
					glm::dot(to_point, to_point) - 1.0f,
					t,
					root
				)
			)
			{
				t = root;
				collision_found = true;
				collision_point = point;
			}
		}

		//C
		{
			float root;
			const glm::vec3& point = triangle.GetVertC();
			const glm::vec3 to_point = point - location;

			if (
				HasLowestRoot(
					velocity_length_sqrd,
					2.0f * (glm::dot(velocity, location - point)),
					glm::dot(to_point, to_point) - 1.0f,
					t,
					root
				)
			)
			{
				t = root;
				collision_found = true;
				collision_point = point;
			}
		}

		//Edges
		//A - B
		{
			float root;
			const glm::vec3& point_0 = triangle.GetVertA();
			const glm::vec3& point_1 = triangle.GetVertB();

			const glm::vec3 edge = point_1 - point_0;
			const glm::vec3 to_point = point_0 - location;

			const float edge_length_sqrd = glm::dot(edge, edge);
			const float edge_dot_velocity = glm::dot(edge, velocity);
			const float edge_dot_to_point = glm::dot(edge, to_point);

			if (
				HasLowestRoot(
					edge_length_sqrd * -velocity_length_sqrd + edge_dot_velocity * edge_dot_velocity,
					edge_length_sqrd * (2.0f * glm::dot(velocity, location)) - 2.0f * edge_dot_velocity * edge_dot_to_point,
					edge_length_sqrd * (1.0f - glm::dot(to_point, to_point)) + edge_dot_to_point * edge_dot_to_point,
					t,
					root
					)
				)
			{
				//Check if in line segment
				float f = (edge_dot_velocity * root - edge_dot_to_point) / edge_length_sqrd;

				if (f >= 0.0f && f <= 1.0f)
				{
					t = root;
					collision_found = true;
					collision_point = point_0 + f * edge;
				}
			}
		}

		//B - C
		{
			float root;
			const glm::vec3& point_0 = triangle.GetVertB();
			const glm::vec3& point_1 = triangle.GetVertC();

			const glm::vec3 edge = point_1 - point_0;
			const glm::vec3 to_point = point_0 - location;

			const float edge_length_sqrd = glm::dot(edge, edge);
			const float edge_dot_velocity = glm::dot(edge, velocity);
			const float edge_dot_to_point = glm::dot(edge, to_point);

			if (
				HasLowestRoot(
					edge_length_sqrd * -velocity_length_sqrd + edge_dot_velocity * edge_dot_velocity,
					edge_length_sqrd * (2.0f * glm::dot(velocity, location)) - 2.0f * edge_dot_velocity * edge_dot_to_point,
					edge_length_sqrd * (1.0f - glm::dot(to_point, to_point)) + edge_dot_to_point * edge_dot_to_point,
					t,
					root
					)
				)
			{
				//Check if in line segment
				float f = (edge_dot_velocity * root - edge_dot_to_point) / edge_length_sqrd;

				if (f >= 0.0f && f <= 1.0f)
				{
					t = root;
					collision_found = true;
					collision_point = point_0 + f * edge;
				}
			}
		}

		//C - A
		{
			float root;
			const glm::vec3& point_0 = triangle.GetVertC();
			const glm::vec3& point_1 = triangle.GetVertA();

			const glm::vec3 edge = point_1 - point_0;
			const glm::vec3 to_point = point_0 - location;

			const float edge_length_sqrd = glm::dot(edge, edge);
			const float edge_dot_velocity = glm::dot(edge, velocity);
			const float edge_dot_to_point = glm::dot(edge, to_point);

			if (
				HasLowestRoot(
					edge_length_sqrd * -velocity_length_sqrd + edge_dot_velocity * edge_dot_velocity,
					edge_length_sqrd * (2.0f * glm::dot(velocity, location)) - 2.0f * edge_dot_velocity * edge_dot_to_point,
					edge_length_sqrd * (1.0f - glm::dot(to_point, to_point)) + edge_dot_to_point * edge_dot_to_point,
					t,
					root
					)
				)
			{
				//Check if in line segment
				float f = (edge_dot_velocity * root - edge_dot_to_point) / edge_length_sqrd;

				if (f >= 0.0f && f <= 1.0f)
				{
					t = root;
					collision_found = true;
					collision_point = point_0 + f * edge;
				}
			}
		}
	}

	if (collision_found) 
	{		
		triangle_hit.distance = glm::distance(location, collision_point);
		triangle_hit.time = t;
		triangle_hit.hit_location = collision_point;
		return true;
	}

	return false;
}