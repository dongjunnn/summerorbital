#pragma once
#include <string>
#include <SDL_ttf.h>
#include <SDL.h>

struct LabelComponent	// text stuff requires way too much information
{
	std::string text;
	TTF_Font* font;
	SDL_Color colour = { 255, 255, 255, 255 }; // default white
	bool focused = false;

	int offset_x = 0;
	int offset_y = 0;

	bool toUpdate = true;
	SDL_Texture* cache = nullptr;

	LabelComponent(std::string text, TTF_Font* font, SDL_Color colour, bool focused, int offset_x, int offset_y, bool toUpdate)
		: text(text), font(font), colour(colour), focused(focused), offset_x(offset_x), offset_y(offset_y), toUpdate(toUpdate) {}

	LabelComponent(std::string text, TTF_Font* font)
		: text(text), font(font) {}

	~LabelComponent()
	{
		if (cache)
		{
			SDL_DestroyTexture(cache);
			cache = nullptr;
		}
	}

	void update(std::string txt, SDL_Color color)
	{
		text = txt;
		colour = color;

		toUpdate = true;
	}

	void update(std::string txt)
	{
		text = txt;
		
		toUpdate = true;
	}

	void update(SDL_Color color)
	{
		colour = color;

		toUpdate = true;
	}
};

inline std::ostream& operator<<(std::ostream& os, const LabelComponent& component)
{
	os << "Label component currently not supported";

	return os;
}