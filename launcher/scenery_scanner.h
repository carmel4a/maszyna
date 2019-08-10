#pragma once

#include <filesystem>
#include "Texture.h"
#include "utilities.h"
#include "parser.h"

struct trainset_desc {
	std::pair<int, int> file_bounds;

	std::string description;

	std::string name;
	std::string track;

	float offset { 0.f };
	float velocity { 0.f };

	std::vector<std::pair<std::string, int>> vehicles;
};

struct scenery_desc {
	std::filesystem::path path;
	std::string name;
	std::string description;
	std::string image_path;
	texture_handle image = 0;

	std::vector<std::pair<std::string, std::string>> links;
	std::vector<trainset_desc> trainsets;

	std::string title() {
		if (!name.empty())
			return name;
		else
			return path.stem().string();
	}
};

class scenery_scanner {
public:
	std::vector<scenery_desc> scenarios;

	void scan();

private:
	void scan_scn(std::filesystem::path path);
	void parse_trainset(cParser &parser);
};
