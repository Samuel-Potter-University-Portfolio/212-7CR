#pragma once
#include "API.h"
#include "Element.h"


#include <vector>


enum CanvasMode
{
	PixelPerfect,
	Scaled
};

class POOT_API Canvas : public Element
{
private:
	std::vector<Element*> elements;


public:
	Canvas();
	virtual ~Canvas();
	bool cleanup_children = true;
	CanvasMode canvas_mode = PixelPerfect;

	virtual void Render(RenderRequest& request, float tick_time);

protected:
	virtual void LogicTick(float delta_time) override;
	virtual void WindowTick(float delta_time) override;

public:
	void AddElement(Element* element);

	template<typename Elem>
	Elem* MakeElement()
	{
		Elem* elem = new Elem;
		AddElement(elem);
		return elem;
	}
};

