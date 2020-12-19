#include "pch.h"
#include "FLIC.h"
#include <algorithm>
#include <vector>
#include <assert.h>
FLIC::FLIC()
	: _components(nullptr)
	, _width(0)
	, _height(0)
	, _channel(0)
	, _xstrips(0)
	, _ystrips(0)
	, _numsp(0)
	, _pImgLabels(nullptr)
	, m(0.0f)
	, itr(0)
	, move_number(0)
	, spcount(0)
{

	pProcessImg = new CDib();
	//pTempImg = new CDib();
}
FLIC::FLIC(CDib* pDib)
	: _components(nullptr)
	, _width(0)
	, _height(0)
	, _channel(0)
	, _xstrips(0)
	, _ystrips(0)
	, _numsp(0)
	, m(0.0f)
	, itr(0)
	, move_number(0)
	, spcount(0)
{
	pProcessImg = new CDib(*pDib);
	//pTempImg = new CDib(*pDib);
	_pImgLabels = new int[pDib->GetHeight() * pDib->GetWidth()];
}
FLIC::~FLIC()
{
	if (pProcessImg)
	{
		delete pProcessImg;
		pProcessImg = nullptr;
	}
	if (_pImgLabels)
	{
		delete [] _pImgLabels;
	}
	//delete pTempImg;
	//pTempImg = nullptr;
}
CDib* FLIC::showLabels(int* finalLabels)
{
	CDib* pTest = new CDib();
	pTest->createGradeBmp(_width, _height, false);
	for (int w = 0; w < _width; ++w)
	{
		for (int h = 0; h < _height; ++h)
		{
			*(pTest->GetData() + h * _width + w) = (BYTE)(finalLabels[h * _width + w] * 255 / spcount);
		}
	}
	return pTest;
}
void FLIC::superPixels(CDib* lpDib, CDib** m_pAllImages, int superpixels)
{
	*pProcessImg = *lpDib;
	m = 20;
	spcount = superpixels;
	pProcessImg->RgbToLab();
	if(nullptr == _pImgLabels)
		_pImgLabels = new int[pProcessImg->GetHeight() * pProcessImg->GetWidth()];
	memset(_pImgLabels, 0, sizeof(int) * pProcessImg->GetHeight() * pProcessImg->GetWidth());
	initLabels(pProcessImg, superpixels);
	update_components();
	for (int i = 0; i < 3; i++)
	{
		update_score();
		forward_clustering(pProcessImg, _pImgLabels);
		update_components();
	}
	//pProcessImg->LabToRgb();
	int numLabels;
	int* finalLabels = new int[_width * _height];
	memset(finalLabels, 0, sizeof(int) * _width * _height);
	enforce_label_connectivity(_pImgLabels, _width, _height, finalLabels, numLabels);
	_numsp = numLabels;
	
	unsigned int* ubuff = new unsigned int[_width * _height];
	memset(ubuff, 0, sizeof(unsigned int) * _width * _height);
	draw_borders_manifold(ubuff, finalLabels);
	CDib* pTest = showLabels(finalLabels);
	transform_to_img(ubuff, pProcessImg->GetData());
	m_pAllImages[1] = pProcessImg;
	m_pAllImages[2] = pTest;
	pTest->m_title = "label";
	pProcessImg->m_title = "FLIC";
}
//=============================================
//函数功能：初始化	Initialize labels for pixels according to their locations.
//			the desired number of superpixels K,		超像素个数
//输入参数：输入图像CDib* lpDib,图像数组指针 CDib** m_pAllImages, 超像素个数
//返回值：无
//==============================================
void FLIC::initLabels(CDib* lpDib,  int superpixels)
{
	_width = lpDib->GetWidth();
	_height = lpDib->GetHeight();
	_channel = lpDib->GetDepth();//得到图像通道数灰度为1、RGB为3
	spcount = superpixels;
	const int superpixelsize = 0.5 + double(_width * _height) / double(superpixels);//起始阶段每个超像素中像素数量
	const int step = sqrt(double(superpixelsize)) + 0.5;//步长=超像素中像素数开平方
	m = 1.0 / ((step / m) * (step / m));
	CSize stepSize;
	stepSize.cx = step;
	stepSize.cy = step;
	_xstrips = (0.5 + (double)_width) / stepSize.cx;//x方向的超像素个数
	_ystrips = (0.5 + (double)_height) / stepSize.cy;//y方向的超像素个数
	int xerr = _width - stepSize.cx *_xstrips;//x方向误差（_xstrips个超像素时多出或者少出的像素数）
	if (xerr < 0)//修正_xstrips，并重新计算x方向误差
	{
		--_xstrips;
		xerr = _width - stepSize.cx *_xstrips;
	}
	int yerr = _height - stepSize.cy *_ystrips;
	if (yerr < 0)
	{
		--_ystrips;
		yerr = _height - stepSize.cy *_ystrips;
	}
	double xerrperstrip = double(xerr) / double(_xstrips);//得到每个超像素对应的误差
	double yerrperstrip = double(yerr) / double(_ystrips);
	_numsp = _xstrips * _ystrips;
	
	// Initialize indexes of each sp
	int label = 0;

	for (int y = 0; y < _ystrips; ++y)
	{
		for (int x = 0; x < _xstrips; ++x)
		{
			int wstart = x * stepSize.cx + x * xerrperstrip;//每个x方向超像素起始位置
			int wend = (x + 1) * stepSize.cx + (x + 1) * xerrperstrip;//每个x方向超像素结束位置
			int hstart = y * stepSize.cy + y * yerrperstrip;//每个y方向超像素起始位置
			int hend = (y + 1) * stepSize.cy + (y + 1) * yerrperstrip;//每个y方向超像素结束位置
			if (x == 0)
				wstart = 0;
			if (y == 0)
				hstart = 0;
			if (x == _xstrips - 1)
				wend = _width;
			if (y == _ystrips - 1)
				hend = _height;

			label = y * _xstrips + x;
			
			for (int h = hstart; h < hend; ++h)
			{
				int* pImgRowLabels = _pImgLabels +_width * h ;
				for (int w = wstart; w < wend; ++w)
				{
					pImgRowLabels[w] = label;
				}
			}
		}
	}
}
//=============================================
//函数功能：更新components
//输入参数：无
//返回值：无
//==============================================
void FLIC::update_components()
{
	if (_components)
		delete[] _components;
	_components = new Component[_numsp];
	for (int i = 0; i < _numsp; ++i)
	{
		_components[i].score = 0.0f;
		_components[i].lMean = 0;
		_components[i].aMean = 0;
		_components[i].bMean = 0;
		_components[i].xMean = 0;
		_components[i].yMean = 0;
		_components[i].num = 0;
		_components[i].label = -1;
		memset(_components[i].M_dist, 0, 9 * sizeof(double));
		memset(_components[i].lHist, 0, 256 * sizeof(int));
		memset(_components[i].aHist, 0, 256 * sizeof(int));
		memset(_components[i].bHist, 0, 256 * sizeof(int));
	}
	//int TestLabel[100] = { 0 };
	for (int w = _width - 1; w >= 0; --w)
	{
		for (int h = _height - 1; h >= 0; --h)
		{
			int label = _pImgLabels[_width * h + w];
			//TestLabel[label] = 1;
			_components[label].label = label;
			_components[label].start.x = w;
			_components[label].start.y = h;
			_components[label].xMean += w;
			_components[label].yMean += h;
			_components[label].num += 1;
			double* lpTmpData = pProcessImg->GetDData() + _width * h * 3 + w * 3;
			_components[label].lHist[(int)(*(lpTmpData) * 2.55)] += 1;
			_components[label].aHist[(int)*(lpTmpData + 1) + 128] += 1;
			_components[label].bHist[(int)*(lpTmpData + 2) + 128] += 1;
			_components[label].min_x = w;
		}
	}
	for (int w = 0; w < _width; ++w)
	{
		for (int h = 0; h < _height; ++h)
		{
			int label = _pImgLabels[_width * h + w];
			_components[label].label = label;
			_components[label].max_x = w;
		}
	}
	for (int h = _height - 1; h >= 0; --h)
	{
		for (int w = 0; w < _width; ++w)
		{
			int label = _pImgLabels[_width * h + w];
			_components[label].label = label;
			_components[label].min_y = h;
		}
	}
	for (int h = 0; h < _height; ++h)
	{
		for (int w = 0; w < _width; ++w)
		{
			int label = _pImgLabels[_width * h + w];
			_components[label].label = label;
			_components[label].max_y = h;
		}
	}
	for (int i = 0; i < _numsp; ++i)
	{
		update_sp_center(_components[i]);
	}
}

void FLIC::update_sp_center(Component& component)
{

	int lSum = 0;
	int aSum = 0;
	int bSum = 0;
	if (component.num > 0)
	{
		for (int i = 0; i < MAX_PIXEL_VALUE; ++i)
		{
			lSum += component.lHist[i] * i;
			aSum += component.aHist[i] * i;
			bSum += component.bHist[i] * i;
		}
		component.lMean = lSum / component.num;
		component.aMean = aSum / component.num;
		component.bMean = bSum / component.num;
		component.xMean /= component.num;
		component.yMean /= component.num;

	}
}
void FLIC::score_each_sp(Component& component)
{
	double lScore = 0.0;
	double aScore = 0.0;
	double bScore = 0.0;
	if (component.num > 0)
	{
		for (int i = 0; i < MAX_PIXEL_VALUE; ++i)
		{
			if (component.lHist[i] != 0)
				lScore += (i - component.lMean) * (i - component.lMean) * component.lHist[i];
			if (component.aHist[i] != 0)
				aScore += (i - component.aMean) * (i - component.aMean) * component.aHist[i];
			if (component.bHist[i] != 0)
				bScore += (i - component.bMean) * (i - component.bMean) * component.bHist[i];

		}
		lScore = lScore / component.num;
		aScore = aScore / component.num;
		bScore = bScore / component.num;
		component.score = (lScore + aScore + bScore) / 3;
	}
}

bool compare_components(Score& a, Score& b)
{
	return a.score > b.score;
}
void FLIC::forward_clustering(CDib* img, int* imgLabels)
{
	const int numDirections = 4;
	const int sz = _height * _width;
	const int dxs[numDirections] = { -1, 0, 1, 0 };
	const int dys[numDirections] = { 0, -1, 0, 1 };
	//const int dxs[numDirections] = { -1, -1, 0, 1, 1, 1, 0, -1 };
	//const int dys[numDirections] = { 0, -1, -1, -1, 0, 1, 1, 1 };
	Score* scores = new Score[_numsp];
	int sn = 0;
	// Score each component and then sort them

	for (int i = 0; i < _numsp; ++i)
	{
		scores[i].label = _components[i].label;
		scores[i].score = _components[i].score;
	}
	std::sort(scores, scores + _numsp, compare_components);

	int* xbuff = new int[sz];
	memset(xbuff, 0, sizeof(int) * sz);
	int* ybuff = new int[sz];
	memset(ybuff, 0, sizeof(int) * sz);
	for (int i = 0; i < _numsp; ++i)
	{
		/*if (scores[i].label<0 || scores[i].label>_numsp)
		{
			continue;
		}*/
		Component& component = _components[scores[i].label];

		int count = 1;
		xbuff[0] = component.min_x;
		
		ybuff[0] = component.min_y;

		if (ybuff[0] < 0 || ybuff[0] >= _height)
		{
			cout << ybuff[0] << endl;
		}
		if (xbuff[0] < 0 || xbuff[0] >= _width)
		{
			cout << xbuff[0] << endl;
		}

		int x = xbuff[0];
		int y = ybuff[0];
		while (true)
		{
			x = xbuff[0];
			while (true)
			{
				if ((x >= 0 && x < _width) && (y >= 0 && y < _height) && (component.label == imgLabels[y * _width + x]))
				{
					xbuff[count] = x;
					ybuff[count] = y;
					++count;
				}
				if (x == component.max_x)
					break;
				x++;
			}
			if (y == component.max_y)
				break;
			y++;
		}
		//}

		int label = component.label;
		Pixel point;
		for (int c = 0; c < count; ++c)
		{
			int labels[numDirections] = { 0 };  // Left, right, top, bottom
			double minDiff = 1.0e10;
			int finalLabel = 0;
			labels[0] = imgLabels[ybuff[c] * _width + max(0, xbuff[c] - 1)];
			labels[1] = imgLabels[ybuff[c] * _width + min(_width - 1, xbuff[c] + 1)];
			labels[2] = imgLabels[max(0, ybuff[c] - 1) * _width + xbuff[c]];
			labels[3] = imgLabels[min(_height - 1, ybuff[c] + 1) * _width + xbuff[c]];
			//	labels[4] = imgLabels.at<int>(max(0,ybuff[c] -1), max(0, xbuff[c] - 1));
			//	labels[5] = imgLabels.at<int>(max(0, ybuff[c] - 1), min(_width - 1, xbuff[c] + 1));
			//	labels[6] = imgLabels.at<int>(min(_height - 1, ybuff[c] + 1), max(0, xbuff[c] - 1));
			//	labels[7] = imgLabels.at<int>(min(_height - 1, ybuff[c] + 1), min(_width - 1, xbuff[c] + 1)); 

			if (labels[0] == labels[1] && labels[2] == labels[3] && labels[0] == labels[2] && label == labels[0])
				continue;
			double* lpTmpData = img->GetDData() + _width * 3 * ybuff[c] + xbuff[c]* 3;
			point.l = *(lpTmpData) * 2.55;
			point.a = *(lpTmpData + 1) + 128;
			point.b = *(lpTmpData + 2) + 128;
			point.x = xbuff[c];
			point.y = ybuff[c];
			for (int j = 0; j < numDirections; ++j)
			{
				double diff = 0;
				if (false)
					diff = compute_distance(_components[labels[j]], point, _components[labels[j]].num, 1);
				else
					diff = compute_distance(_components[labels[j]], point, _components[labels[j]].num, 0);

				if (diff < minDiff)
				{
					minDiff = diff;
					finalLabel = labels[j];
				}
			}

			if (finalLabel == label)
				continue;
			imgLabels[ybuff[c] * _width + xbuff[c]] = finalLabel;

			_components[finalLabel].lMean = (_components[finalLabel].lMean * _components[finalLabel].num + point.l) / (_components[finalLabel].num + 1);
			_components[finalLabel].aMean = (_components[finalLabel].aMean * _components[finalLabel].num + point.a) / (_components[finalLabel].num + 1);
			_components[finalLabel].bMean = (_components[finalLabel].bMean * _components[finalLabel].num + point.b) / (_components[finalLabel].num + 1);
			_components[finalLabel].xMean = (_components[finalLabel].xMean * _components[finalLabel].num + point.x) / (_components[finalLabel].num + 1);
			_components[finalLabel].yMean = (_components[finalLabel].yMean * _components[finalLabel].num + point.y) / (_components[finalLabel].num + 1);
			_components[finalLabel].num += 1;

			if (_components[label].num > 1)
			{
				_components[label].lMean = (_components[label].lMean * _components[label].num - point.l) / (_components[label].num - 1);
				_components[label].aMean = (_components[label].aMean * _components[label].num - point.a) / (_components[label].num - 1);
				_components[label].bMean = (_components[label].bMean * _components[label].num - point.b) / (_components[label].num - 1);
				_components[label].xMean = (_components[label].xMean * _components[label].num - point.x) / (_components[label].num - 1);
				_components[label].yMean = (_components[label].yMean * _components[label].num - point.y) / (_components[label].num - 1);
			}
			--_components[label].num;


			// Update start point
			update_start_point(_components[finalLabel], point);

		}

		count = 1;
		memset(xbuff, 0, sizeof(int) * sz);
		memset(ybuff, 0, sizeof(int) * sz);
		xbuff[0] = component.max_x;
		ybuff[0] = component.max_y;

		if (ybuff[0] < 0 || ybuff[0] >= _height)
		{
			cout << ybuff[0] << endl;
		}
		if (xbuff[0] < 0 || xbuff[0] >= _width)
		{
			cout << xbuff[0] << endl;
		}

		x = xbuff[0];
		y = ybuff[0];
		while (true)
		{
			x = xbuff[0];
			while (true)
			{
				if ((x >= 0 && x < _width) && (y >= 0 && y < _height) && (component.label == imgLabels[y * _width + x]))
				{
					xbuff[count] = x;
					ybuff[count] = y;
					++count;
				}
				if (x == component.min_x)
					break;
				x--;
			}
			if (y == component.min_y)
				break;
			y--;
			//}

		}

		label = component.label;
		for (int c = 0; c < count; ++c)
		{
			int labels[numDirections] = { 0 };  // Left, right, top, bottom
			double minDiff = 1.0e10;
			int finalLabel = 0;
			labels[0] = imgLabels[ybuff[c] * _width + max(0, xbuff[c] - 1)];
			labels[1] = imgLabels[ybuff[c] * _width + min(_width - 1, xbuff[c] + 1)];
			labels[2] = imgLabels[max(0, ybuff[c] - 1) * _width + xbuff[c]];
			labels[3] = imgLabels[min(_height - 1, ybuff[c] + 1) * _width + xbuff[c]];
			//	labels[4] = imgLabels.at<int>(max(0, ybuff[c] - 1), max(0, xbuff[c] - 1));
			//	labels[5] = imgLabels.at<int>(max(0, ybuff[c] - 1), min(_width - 1, xbuff[c] + 1));
			//	labels[6] = imgLabels.at<int>(min(_height - 1, ybuff[c] + 1), max(0, xbuff[c] - 1));
			//	labels[7] = imgLabels.at<int>(min(_height - 1, ybuff[c] + 1), min(_width - 1, xbuff[c] + 1));

			if (labels[0] == labels[1] && labels[2] == labels[3] && labels[0] == labels[2] && label == labels[0])
				continue;
			double* lpTmpData = img->GetDData() + _width * ybuff[c] + xbuff[c];
			point.l = *(lpTmpData);
			point.a = *(lpTmpData + 1);
			point.b = *(lpTmpData + 2);
			point.x = xbuff[c];
			point.y = ybuff[c];

			for (int j = 0; j < numDirections; ++j)
			{
				double diff = compute_distance(_components[labels[j]], point, _components[labels[j]].num, 1);

				if (diff < minDiff)
				{
					minDiff = diff;
					finalLabel = labels[j];
				}
			}
			//cout << minDiff << endl;
			if (finalLabel == label)
				continue;
			move_number++;
			imgLabels[ybuff[c] * _width + xbuff[c]] = finalLabel;


			if (true)
			{
				_components[finalLabel].xMean = (_components[finalLabel].xMean * _components[finalLabel].num + point.x) / (_components[finalLabel].num + 1);
				_components[finalLabel].yMean = (_components[finalLabel].yMean * _components[finalLabel].num + point.y) / (_components[finalLabel].num + 1);
				_components[finalLabel].num += 1;

				if (_components[label].num > 1)
				{
					_components[label].xMean = (_components[label].xMean * _components[label].num - point.x) / (_components[label].num - 1);
					_components[label].yMean = (_components[label].yMean * _components[label].num - point.y) / (_components[label].num - 1);
				}
				_components[label].num -= 1;


				// Update start point
				update_start_point(_components[finalLabel], point);
			}
			else
			{
				_components[finalLabel].lMean = (_components[finalLabel].lMean * _components[finalLabel].num + point.l) / (_components[finalLabel].num + 1);
				_components[finalLabel].aMean = (_components[finalLabel].aMean * _components[finalLabel].num + point.a) / (_components[finalLabel].num + 1);
				_components[finalLabel].bMean = (_components[finalLabel].bMean * _components[finalLabel].num + point.b) / (_components[finalLabel].num + 1);
				_components[finalLabel].xMean = (_components[finalLabel].xMean * _components[finalLabel].num + point.x) / (_components[finalLabel].num + 1);
				_components[finalLabel].yMean = (_components[finalLabel].yMean * _components[finalLabel].num + point.y) / (_components[finalLabel].num + 1);
				_components[finalLabel].num += 1;

				if (_components[label].num > 1)
				{
					_components[label].lMean = (_components[label].lMean * _components[label].num - point.l) / (_components[label].num - 1);
					_components[label].aMean = (_components[label].aMean * _components[label].num - point.a) / (_components[label].num - 1);
					_components[label].bMean = (_components[label].bMean * _components[label].num - point.b) / (_components[label].num - 1);
					_components[label].xMean = (_components[label].xMean * _components[label].num - point.x) / (_components[label].num - 1);
					_components[label].yMean = (_components[label].yMean * _components[label].num - point.y) / (_components[label].num - 1);
				}
				_components[label].num -= 1;

				update_start_point(_components[finalLabel], point);
			}
		}

	}
	delete[] scores;
	delete[] xbuff;
	delete[] ybuff;
}

void FLIC::enforce_label_connectivity(const int* labels, const int	width, 
	const int height, int*& nlabels, int& numlabels)
{
	//	const int dx8[8] = {-1, -1,  0,  1, 1, 1, 0, -1};
	//	const int dy8[8] = { 0, -1, -1, -1, 0, 1, 1,  1};

	const int dx4[8] = { -1, 0, 1, 0 , -1 , -1 ,1 ,1 };
	const int dy4[8] = { 0, -1, 0, 1 , -1 ,1 ,-1 ,1 };
	/*const int dx4[4] = { -1, 0, 1, 0 };
	const int dy4[4] = { 0, -1, 0, 1 };*/
	const int sz = width * height;
	//nlabels.resize(sz, -1);
	for (int i = 0; i < sz; i++)
		nlabels[i] = -1;
	int label(0);
	int* xvec = new int[sz];
	int* yvec = new int[sz];
	int oindex(0);
	int adjlabel(0);//adjacent label
	for (int j = 0; j < height; j++)
	{
		for (int k = 0; k < width; k++)
		{
			if (0 > nlabels[oindex])
			{
				nlabels[oindex] = label;
				//--------------------
				// Start a new segment
				//--------------------
				xvec[0] = k;
				yvec[0] = j;
				//-------------------------------------------------------
				// Quickly find an adjacent label for use later if needed
				//-------------------------------------------------------
				for (int n = 0; n < 8; n++)
				{
					int x = xvec[0] + dx4[n];
					int y = yvec[0] + dy4[n];
					if ((x >= 0 && x < width) && (y >= 0 && y < height))
					{
						int nindex = y * width + x;
						if (nlabels[nindex] >= 0)
							adjlabel = nlabels[nindex];
					}
				}

				int count(1);
				for (int c = 0; c < count; c++)
				{
					for (int n = 0; n < 8; n++)
					{
						int x = xvec[c] + dx4[n];
						int y = yvec[c] + dy4[n];

						if ((x >= 0 && x < width) && (y >= 0 && y < height))
						{
							int nindex = y * width + x;

							if (0 > nlabels[nindex] && labels[oindex] == labels[nindex])
							{
								xvec[count] = x;
								yvec[count] = y;
								nlabels[nindex] = label;
								count++;
							}
						}

					}
				}
				//-------------------------------------------------------
				// If segment size is less then a limit, assign an
				// adjacent label found before, and decrement label count.
				//-------------------------------------------------------
				if (count <= (double)(_width * _height) / spcount / 5)
				{
					for (int c = 0; c < count; c++)
					{
						int ind = yvec[c] * width + xvec[c];
						nlabels[ind] = adjlabel;
					}
					label--;
				}
				label++;
			}
			oindex++;
		}
	}
	numlabels = label;

	if (xvec) delete[] xvec;
	if (yvec) delete[] yvec;
}

void FLIC::draw_borders_manifold(unsigned int*& ubuff, int*& labels)
{
	const int dx8[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
	const int dy8[8] = { 0, -1, -1, -1, 0, 1, 1, 1 };
	/*const int dx4[4] = { -1, 1, 0, 0 };
	const int dy4[4] = { 0, 0, -1, 1 };*/
	int width = _width;
	int height = _height;
	int sz = width * height;
	vector<bool> istaken(sz, false);
	vector<int> contourx(sz); vector<int> contoury(sz);
	int mainindex(0); int cind(0);
	for (int j = 0; j < height; j++)
	{
		for (int k = 0; k < width; k++)
		{
			int np(0);
			for (int i = 0; i < 8; i++)
			{
				int x = k + dx8[i];
				int y = j + dy8[i];

				if ((x >= 0 && x < width) && (y >= 0 && y < height))
				{
					int index = y * width + x;
					//if( false == istaken[index] )//comment this to obtain internal contours
					{
						if (labels[mainindex] != labels[index]) np++;
					}
				}
			}
			//if( np > 1 )//change to 2 or 3 for thinner lines
			if (np > 2)
			{
				contourx[cind] = k;
				contoury[cind] = j;
				istaken[mainindex] = true;
				cind++;
			}
			mainindex++;
		}
	}

	/*for (int i = 0; i<sz; i++)
	{
		ubuff[i] = 0xffffff;
	}*/

	int numboundpix = cind;//int(contourx.size());
	for (int j = 0; j < numboundpix; j++)
	{
		int ii = contoury[j] * width + contourx[j];
		ubuff[ii] = 255;
	}
}

void FLIC::transform_to_img(unsigned int* buff, LPBYTE lpData)
{
	LONG lineByte = pProcessImg->GetLineByte();
	for (int h = 0; h < _height; ++h)
	{
		for (int w = 0; w < _width; ++w)
		{
			unsigned int value = buff[h * _width + w];
			if (value != 0)
			{
				lpData[h * lineByte + w * 3 + 2] = 0;
				lpData[h * lineByte + w * 3] = 0;
				lpData[h * lineByte + w * 3 + 1] = 0;
			}
			
		}
	}
}