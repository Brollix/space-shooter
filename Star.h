#pragma once
class Star
{
public:
	Vector2f pos;
	Vector2f deltaPos;
	Shader* shader;

	CircleShape star;
	float starSize;

	Star(
		int starSize,
		bool colorSwitch,
		Vector2f rndPos
	) {
		this->starSize = (float)starSize;
		star.setRadius(starSize);
		star.setPosition(rndPos);
		pos = getPos();



		if (colorSwitch) {
			//blue
			star.setFillColor(Color(190, 212, 250));
		}
		else {
			//orange
			star.setFillColor(Color(255, 246, 222));
		}
	}

	void update(float dt) {
		move(dt);
	}

	void render(RenderTarget& window) {
		window.draw(star, shader);
	}

	void applyShader(sf::Shader& shader) {
		this->shader = &shader;
	}

	void move(float dt) {
		Vector2f deltaPos;
		float speed = starSize * 25;

		if (Keyboard::isKeyPressed(Keyboard::W)) {
			deltaPos.y -= 1;
		}
		if (Keyboard::isKeyPressed(Keyboard::A)) {
			deltaPos.x -= 1;
		}
		if (Keyboard::isKeyPressed(Keyboard::S)) {
			deltaPos.y += 1;
		}
		if (Keyboard::isKeyPressed(Keyboard::D)) {
			deltaPos.x += 1;
		}

		pos -= normalize(deltaPos) * speed * dt;

		setPos(pos);
	}

	float getRadius() {
		return star.getRadius();
	}

	void setRadius(float radius) {
		star.setRadius(radius);
	}

	Vector2f getPos() {
		return star.getPosition();
	}

	void setPos(Vector2f newPos) {
		star.setPosition(newPos);
	}

	void setFillColor(Color color) {
		star.setFillColor(color);
	}

	float magnitude(Vector2f vec) {
		return sqrt(pow(vec.x, 2) + pow(vec.y, 2));
	}

	Vector2f normalize(Vector2f vec) {
		float mag = magnitude(vec);
		if (mag) {
			vec /= (float)mag;
		}
		return vec;
	}
};


