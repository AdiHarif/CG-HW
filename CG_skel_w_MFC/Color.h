#pragma once

#define WHITE {1.0,1.0,1.0}
#define INACTIVE_GRAY {0.4, 0.4, 0.4}

#define GREEN { 0.0, 1.0, 0.0 }
#define PURPLE {1.0,0,1.0}
#define RED {1.0, 0, 0}

typedef struct s_color {
	float r = 0.0;
	float g = 0.0;
	float b = 0.0;
} Color;
