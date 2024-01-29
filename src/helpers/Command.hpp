#pragma once
#include "renderer/Light.h"

class EditorCommand
{
public:
	virtual void add() = 0;
	virtual void undo() = 0;
	virtual ~EditorCommand() {}

};



class AddLightCommand : public EditorCommand
{
private:


public:
	virtual void add() override
	{

	}
	virtual void undo() override
	{

	}

};