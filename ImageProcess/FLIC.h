#pragma once
#include "Dib.h"
#define MAX_PIXEL_VALUE 256
#define MAX_VALUE 35536
struct Score
{
	int label;
	double score;
};
class Utils
{
public:
	template <class T>
	static T square(const T& a) { return a * a; }
};
struct Pixel
{
	int index;
	int x;
	int y;
	union {
		int l;
		int r;
	};
	union {
		int a;
		int g;
	};
	int b;
};

struct Component
{
	double score;
	int num;
	int num_spa;
	int label;
	double lMean;
	double lMean_t;
	double aMean;
	double aMean_t;
	double bMean;
	double bMean_t;
	double xMean;
	double xMean_t;
	double yMean;
	double yMean_t;
	int lHist[MAX_PIXEL_VALUE];
	int aHist[MAX_PIXEL_VALUE];
	int bHist[MAX_PIXEL_VALUE];
	Pixel start;
	int max_x;
	int min_x;
	int max_y;
	int min_y;
	double M_dist[9];
};

class FLIC
{
public:
	FLIC();
	FLIC(CDib* pDib);
	~FLIC();
	
	int move_number;
	int spcount;
	int itr;

	void initLabels(CDib* lpDib, int superpixels);
	
	void update_components();
	void update_sp_center(Component& component);
	void score_each_sp(Component& component);
	inline void update_score();
	void forward_clustering(CDib* img, int* imgLabels);
	inline double compute_distance(const Component& center, const Pixel& point, const double& area, int sign);
	inline void update_start_point(Component& component, Pixel& point);
	inline double compute_color_distance(const Pixel& point, const Pixel& point2);
	void enforce_label_connectivity(const int* labels, const int	width, const int height, int*& nlabels, int& numlabels);
	void draw_borders_manifold(unsigned int*& ubuff, int*& labels);
	void transform_to_img(unsigned int* buff, LPBYTE lpData);
	CDib* showLabels(int* finalLabels);

	void superPixels(CDib* lpDib, CDib** m_pAllImages, int superpixels);
	
private:
	Component* _components;
	CDib* pProcessImg;
	//CDib* pTempImg;
	int _width;
	int _height;
	int _xstrips;
	int _ystrips;
	int _channel;
	int _numsp;
	int* _pImgLabels;
	double m;
};

inline void FLIC::update_score()
{
	for (int i = 0; i < _numsp; ++i)
	{
		score_each_sp(_components[i]);
	}
}

inline double FLIC::compute_distance(const Component& center, const Pixel& point, 
	const double& area, int sign)
{
	double colorDiff = Utils::square(center.lMean - point.l) + Utils::square(center.aMean - point.a)
		+ Utils::square(center.bMean - point.b);
	double spaceDiff = Utils::square(center.xMean - point.x) + Utils::square(center.yMean - point.y);
	double diff = 0.f;
	if (sign == 0)
		diff = colorDiff;
	else if (sign == 1)
	{
		diff = colorDiff + spaceDiff * m;
	}
	return diff;
}
inline void FLIC::update_start_point(Component& component, Pixel& point)
{
	if (point.x > component.max_x)
		component.max_x = point.x;
	if (point.x < component.min_x)
		component.min_x = point.x;
	if (point.y > component.max_y)
		component.max_y = point.y;
	if (point.y < component.min_y)
		component.min_y = point.y;
}
inline double FLIC::compute_color_distance(const Pixel& point, const Pixel& point2)
{
	double colorDiff = Utils::square(point2.l - point.l) + Utils::square(point2.a - point.a) + Utils::square(point2.b - point.b);
	return sqrt(colorDiff);
}

