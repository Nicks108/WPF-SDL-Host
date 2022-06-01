#include "Bitmap.h"

int Bitmap::ObjectCount = 0;

Bitmap::Bitmap(SDL_Renderer* renderer, string filename, int xPos, int yPos, bool useTransparency, const string objectName )
{
	FileName = filename;
	ObjectName = objectName;
	if (objectName == "")
	{
		ObjectName = "Object " + to_string(ObjectCount);
		ObjectCount++;
	}
	

	m_pRenderer = renderer;
	cout << filename.c_str() << endl;
	m_pBitmapSurface = IMG_Load(filename.c_str());
	//m_pBitmapSurface = SDL_LoadBMP(filename.c_str());
	if (!m_pBitmapSurface)
	{
		cerr << "SURFACE for bitmap " << filename << " not loaded" << endl << IMG_GetError() << endl;
		//cerr << "SURFACE for bitmap " << filename << " not loaded" << endl << SDL_GetError() << endl;
	}
	else
	{
		if (useTransparency)
		{
			Uint32 colourKey = SDL_MapRGB(m_pBitmapSurface->format, 255, 0, 255);
			SDL_SetColorKey(m_pBitmapSurface, SDL_TRUE, colourKey);
		}

		m_pBitmapTexture = SDL_CreateTextureFromSurface(m_pRenderer, m_pBitmapSurface);
		if (!m_pBitmapTexture)
		{
			cerr << "Texture for bitmap " << filename << " not loaded" << endl << SDL_GetError() << endl;
		}

		Transfrom.Position().X = xPos;
		Transfrom.Position().Y = yPos;

		Transfrom.Scale().X = m_pBitmapSurface->w;
		Transfrom.Scale().Y = m_pBitmapSurface->h;

		Transfrom.Owner = this;


		for (ComponentBase* Component : Components)
		{
			Component->Start();
		}
	}
}


//Bitmap::Bitmap(const Bitmap& BitmapToCopy)
//{
//	this->m_pRenderer = BitmapToCopy.m_pRenderer;
//	this->m_pBitmapTexture = BitmapToCopy.m_pBitmapTexture;
//	this->m_pBitmapSurface
//}


Bitmap::~Bitmap()
{
	for (ComponentBase* Component : Components)
	{
		Component->End();
	}

	if (m_pBitmapTexture)
		SDL_DestroyTexture(m_pBitmapTexture);

	if (m_pBitmapSurface)
		SDL_FreeSurface(m_pBitmapSurface);
}

void Bitmap::Draw()
{
	Transfrom2D updatedTrnasfrom = Transfrom.UpateTransform();
	//Draw({ Transfrom.Position.X , Transfrom.Position.Y , m_pBitmapSurface->w, m_pBitmapSurface->h });
	Draw({ updatedTrnasfrom.Position().X , updatedTrnasfrom.Position().Y , updatedTrnasfrom.Scale().X, updatedTrnasfrom.Scale().Y});

    for(Transfrom2D* child: Transfrom.Children)
    {
		Bitmap* ChildAsBitmap = static_cast<Bitmap*>(child->Owner);
		ChildAsBitmap->Draw();
    }
}

void Bitmap::Draw(SDL_Rect destRect)
{
	if (m_pBitmapTexture)
	{
		SDL_RenderCopy(m_pRenderer, m_pBitmapTexture, NULL, &destRect);
	}
}

void Bitmap::Update()
{
	for (ComponentBase* Component : Components)
	{
		Component->Update();
	}

	for (Transfrom2D* child : Transfrom.Children)
	{
		Bitmap* ChildAsBitmap = static_cast<Bitmap*>(child->Owner);
		ChildAsBitmap->Update();
	}

}



