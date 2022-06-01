#pragma once
#include <vector>


struct Vector2
{
	Vector2()
	{
		int X =0;
		int Y =0;
	}
	Vector2(int x, int y)
	{
		X = x;
		y = y;
	}
	int X;
	int Y;
};
typedef Vector2 vec2;


class Transfrom2D
{
private:
	vec2 _Position;
	vec2 _Scale;
	Transfrom2D* _Parent = nullptr;

	//std::vector<Transfrom2D*> Children;

public:
	void* Owner;
	std::vector<Transfrom2D*> Children;

	void ParentSet(Transfrom2D& parent)
	{
		if(_Parent)
		{
			//if has parent, undo transfrom diff to put this back in correct location in space
			Transfrom2D updatedTrnasfrom = UpateTransform();
			this->_Position = updatedTrnasfrom.Position();
			this->_Scale = updatedTrnasfrom.Scale();

			_Parent->RemoveFromParent(this);
			_Parent = nullptr;
		}

		parent.Children.push_back(this);
		_Parent = &parent;

		Transfrom2D Diff = *this - parent;
		_Position = Diff.Position();
		_Scale = Diff.Scale();

	}
	const Transfrom2D&  ParentGet()
	{
		return *_Parent;
	}

	void RemoveFromParent(Transfrom2D* child)
	{
		std::vector<Transfrom2D*>::iterator it;
		it = std::find(Children.begin(), Children.end(), child);

		if (it != Children.end())
		{
			Children.erase(it);
		}

		return;
	}
	

	Transfrom2D()
    {
        
    }
	Transfrom2D(vec2 position, vec2 scale)
	{
		_Position = position;
		_Scale = scale;
	}

	vec2& Position()
	{
		return _Position;
	}
	vec2& Scale()
	{
		return _Scale;
	}

	Transfrom2D UpateTransform()
	{
		Transfrom2D updatedTransform = *this;
		if (this->_Parent != __nullptr)
		{
			Transfrom2D t = this->_Parent->UpateTransform();
			updatedTransform += t;
		}
		return updatedTransform;
	}

	Transfrom2D  operator +(const Transfrom2D& RHS)
	{
		Transfrom2D temp;
		temp.Position().X =  this->Position().X + RHS._Position.X;
		temp.Position().Y =  this->Position().Y + RHS._Position.Y;

		temp._Scale.X = this->_Scale.X + RHS._Scale.X;
		temp._Scale.Y = this->_Scale.Y + RHS._Scale.Y;

		return temp;
	}

	void operator +=(const Transfrom2D& RHS)
	{
		Transfrom2D temp;
		this->Position().X += RHS._Position.X;
		this->Position().Y += RHS._Position.Y;

		this->_Scale.X += RHS._Scale.X;
		this->_Scale.Y += RHS._Scale.Y;

		
	}

	Transfrom2D  operator -(const Transfrom2D& RHS)
	{
		Transfrom2D temp;
		temp.Position().X = this->Position().X - RHS._Position.X;
		temp.Position().Y = this->Position().Y - RHS._Position.Y;

		temp._Scale.X = this->_Scale.X - RHS._Scale.X;
		temp._Scale.Y = this->_Scale.Y - RHS._Scale.Y;

		return temp;
	}

	void operator -=(const Transfrom2D& RHS)
	{
		Transfrom2D temp;
		this->Position().X -= RHS._Position.X;
		this->Position().Y -= RHS._Position.Y;

		this->_Scale.X -= RHS._Scale.X;
		this->_Scale.Y -= RHS._Scale.Y;


	}

};

