#pragma once

enum StateID
{
	Game,
	Menu,	
	None
};

class State
{
public:
	
	virtual StateID inputs() = 0;
	virtual void update(float dt) = 0;
	virtual void draw() = 0;
	virtual void drawGui() = 0;	

	virtual void enter() = 0;
	virtual void exit() = 0;
	
private:

};

