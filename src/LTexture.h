/*
 * LTexture.h
 *
 *  Created on: 6 Aug 2015
 *      Author: eva
 */

#ifndef LTEXTURE_H_
#define LTEXTURE_H_

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string>

using namespace std;

class LTexture {
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	// Set the Renderer for this texture
	void setRenderer(SDL_Renderer* renderer);

	//Creates image from font string
	bool loadFromRenderedText(TTF_Font* font, string textureText,
			SDL_Color textColor);

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void draw(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0,
			SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	// The Renderer the texture will be renderng to
	SDL_Renderer* mRenderer;

	//Image dimensions
	int mWidth;
	int mHeight;
};

#endif /* LTEXTURE_H_ */
