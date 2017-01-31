#pragma once


class Tickable
{
private:
	bool logic_begun = false;
	bool window_begun = false;

protected:
	virtual void LogicBegin() {}
	virtual void WindowBegin() {}

	virtual void LogicTick(float delta_time) {}
	virtual void WindowTick(float delta_time) {}

public:
	void ForceLogicBegin();
	void ForceWindowBegin();

	void HandleLogicTick(float delta_time);
	void HandleWindowTick(float delta_time);


	inline const bool HasLogicBegun() { return logic_begun; }
	inline const bool HasWindowBegun() { return window_begun; }
	inline const bool HasBegun() { return logic_begun && window_begun; }
	inline const bool HasPartiallyBegun() { return logic_begun || window_begun; }
};

