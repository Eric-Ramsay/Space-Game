#pragma once

// Creates a sprite sheet from a file string
sf::Texture createTexture(std::string file) {
	sf::Texture texture;
	texture.loadFromFile(file);
	return texture;
}

// Draws a sprite from the global sprite sheet
void drawSprite(int sX, int sY, int w, int h, float dX, float dY, int scale = 1, float rotation = 0.0f, sf::Color color = sf::Color(255, 255, 255)) {
	static bool init = false;
	static sf::Texture sprites;
	if (!init) {
		sprites = createTexture("./Sprites/sprites.png");
		init = true;
	}
	sf::IntRect rect(sX, sY, w, h);
	sf::Sprite sprite(sprites, rect);
	sprite.setOrigin(w / 2, h / 2);
	sprite.setScale(scale, scale);
	sprite.setPosition(dX, dY);
	sprite.setColor(color);
	sprite.setRotation(rotation);
	window.draw(sprite);
}

int charWidth(char c) {
	//Assumes char width of 5 and char height of 7
	switch (c) {
	case '"':case ',': case '.': case '\'': case '!': case ':': return 1;
	case ';': return 2;
	case ' ': case '[': case ']': case '|': return 3;
	case '\n': return 0;
	default: return 5;
	}
}

sf::Color getColor(std::string text = "") {
	return sf::Color(255, 255, 255);
}

//Prints text onto the screen
int Print(std::string text, float x = -1, float y = -1, float scale = 1.0f, bool center = false, int width = 1920 / 2) {
	int cap = width;
	int TEXT_W = 5;
	int TEXT_H = 7;

	sf::Color p = sf::Color(255, 255, 255);
	if (y == -1) {
		cap = x;
	}
	std::vector<int> splitIndexes;
	std::string line = "";
	bool skipping = false;
	std::string color = "";
	std::vector<int> lineWidth = { 0 };
	int spaces = 0; int splits = 0;
	float drawX = x;
	float drawY = y;
	float offX = 0; float offY = 0;
	float sH = TEXT_H; float sW = TEXT_W;
	int sY = 1;
	int sX = 65;
	bool prnt = true;

	int wordlen = 0;
	for (char& c : text) {
		if (c == '*') {
			skipping = !skipping;
		}
		else if (!skipping) {
			wordlen += scale * (charWidth(c) + 1);
			lineWidth[lineWidth.size() - 1] += scale * (charWidth(c) + 1);
			line += c;
			if (c == ' ' || c == '\n') {
				if (c == '\n' || lineWidth[lineWidth.size() - 1] > cap) {
					line = "";
					if (lineWidth[lineWidth.size() - 1] > cap && spaces > 0) {
						splitIndexes.push_back(spaces - 1);
						lineWidth[lineWidth.size() - 1] -= wordlen;
						lineWidth.push_back(wordlen);
					}
					else {
						splitIndexes.push_back(spaces);
						lineWidth.push_back(0);
					}
				}
				wordlen = 0;
				spaces++;
			}
		}
	}
	if (lineWidth[lineWidth.size() - 1] > cap && spaces > 0) {
		splitIndexes.push_back(spaces - 1);
		lineWidth[lineWidth.size() - 1] -= wordlen;
		lineWidth.push_back(wordlen);
	}
	spaces = 0;
	if (y == -1) {
		return splitIndexes.size();
	}
	if (center) {
		drawX = 1 + x - (lineWidth[splits] / 2.0); //if ur having problems try scale + x
		drawY = y - scale * lineWidth.size() * (1 + (sH / 2.0));
	}
	skipping = false;
	for (char& c : text) {
		sH = TEXT_H; sW = TEXT_W;
		if (c == '*') {
			skipping = !skipping;
			if (!skipping) {
				if (getColor(color) != p) {
					p = getColor(color);
				}
				else {
					p = sf::Color(255, 255, 255);
				}
				color = "";
			}
		}
		else if (!skipping) {
			offX = 0; offY = 0;
			sH = sH; sW = charWidth(c);
			sY = 0; sX = 0;
			prnt = true;
			if (c >= 48 && c <= 57) { //Numbers
				sY = sH + 1;
				sX = ((int)c - 48) * (sW + 1);
			}
			else if (c >= 65 && c <= 90) { //Letters, uppercase(?)
				sY = 0;
				sX = ((int)c - 65) * (sW + 1);
			}
			else if (c >= 97 && c <= 122) { //Letters again, lowercase(?)
				sY = 0;
				sX = ((int)c - 97) * (sW + 1);
			}
			else {
				sY = 17;
				switch (c) {
				case '"': case '\'': sX = 1; sY = 21; sW = 1; sH = 3; break;
				case '.': sX = 118; sW = 1; break;
				case ',': sX = 1; sW = 1; offY = 1; break;
				case '>': sX = 6; break;
				case '?': sX = 18; break;
				case '!': sX = 14; sW = 1; break;
				case '+': sX = 24; break;
				case '-': sX = 30; break;
				case '$': sX = 36; break;
				case ':': sX = 44; sW = 1; break;
				case '<': sX = 84; break;
				case '[': sX = 48; sW = 3; break;
				case ']': sX = 56; sW = 3; break;
				case '#': sX = 60; break;
				case '(': sX = 66; break;
				case ')': sX = 72; break;
				case '=': sX = 78; break;
				case '/': sX = 92; break;
				case '%': sX = 98; break;
				case '|': sX = 60; sW = 3; break;
				case ';': sX = 89; sW = 2; break;
				case ' ': sW = 2; prnt = false;
				case '\n': prnt = false;
				default: prnt = false;
				}
			}
			if (prnt) {
				//drawSprite(int sX, int sY, int w, int h, int dX, int dY, int scale = zoom, sf::Color color = sf::Color(255, 255, 255))
				drawSprite(sX + 320, sY, sW, sH, drawX + offX * scale, drawY + offY * scale, scale, 0, p);
			}
			if (c != '\n') {
				drawX += scale * (sW + offX + 1);
			}
			if (c == ' ' || c == '\n') {
				if (splitIndexes.size() > splits && splitIndexes[splits] == spaces++) {
					drawX = x;
					splits++;
					if (center) {
						drawX -= (lineWidth[splits] / 2);
					}
					drawY += scale * (sH + 3);
				}
			}
		}
		else {
			color += c;
		}
	}
	return splits;
}

void drawPosition(Position p) {
	drawSprite(34, 0, 2, 2, p.x, p.y);
}


void drawFutureTrajectory(Ship s, std::vector<Planet>& plan) {
	for (int i = 0; i < 2000; i++) {
		moveShip(&s);
		for (Planet p : plan) {
			applyGravity(s.pos, p);
		}
		if (i % 20 == 0) {
			drawPosition(s.pos);
		}
	}
}

void drawPath(Ship s, std::vector<Planet> planets) {
	while (s.path.size() > 0) {
		PathPos path = (s.path).front();
		s.path[0].num--;
		if (s.path[0].num <= 0) {
			s.path.pop_front();
		}
		moveShip(&s, path.up, path.down, path.left, path.right);
		for (Planet p : planets) {
			applyGravity(s.pos, p);
		}
		drawSprite(34, 0, 2, 2, s.pos.x, s.pos.y);
	}
}

void drawBullet(Bullet* b) {
	drawSprite(48, 32, 4, 1, b->pos.x, b->pos.y, 1, deg(b->pos.a + rad(90)), b->color);
}

void drawShip(Ship* s) {
	int sX = 0;
	float dX = 0;
	float dY = 0;
	float co = cos(s->pos.a);
	float si = sin(s->pos.a);
	for (int i = 0; i < s->parts.size(); i++) {
		Part p = s->parts[i];
		sX = p.sX + 16 * (p.rotation % 2);
		if (p.thrust > 0 && p.active) {
			sX += 32;
		}
		dX = co * p.x - si * p.y + s->pos.x;
		dY = si * p.x + co * p.y + s->pos.y;
		drawSprite(sX, p.sY + 16 * (p.rotation / 2), 16, 16, dX, dY, 1, deg(s->pos.a));
		if (p.turret) {
			drawSprite(32, 128, 16, 16, dX, dY, 1, deg(p.turret_angle));
		}
	}
}

void drawPlanet(Planet* p, sf::View& view) {
	static bool init = true;
	static sf::Shader planet;

	if (init) {
		init = false;
		if (!planet.loadFromFile("planet.frag", sf::Shader::Fragment)) {
			std::cout << "There was an error loading a fragment shader" << std::endl;
		}
	}

	sf::CircleShape plan(p->size, p->size / 2);
	plan.setOrigin(p->size / 2, p->size / 2);
	plan.setPosition(p->x, p.y);
	float x = plan.getPosition().x;
	float y = plan.getPosition().y;
	window.draw(plan, &planet);
}

