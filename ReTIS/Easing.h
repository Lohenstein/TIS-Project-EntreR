#pragma once
namespace Easing
{
	double InQuad		(double t, double totaltime, double max, double min);
	double OutQuad		(double t, double totaltime, double max, double min);
	double InOutQuad	(double t, double totaltime, double max, double min);
	double InCubic		(double t, double totaltime, double max, double min);
	double OutCubic		(double t, double totaltime, double max, double min);
	double InOutCubic	(double t, double totaltime, double max, double min);
	double InQuart		(double t, double totaltime, double max, double min);
	double OutQuart		(double t, double totaltime, double max, double min);
	double InOutQuart	(double t, double totaltime, double max, double min);
	double InQuint		(double t, double totaltime, double max, double min);
	double OutQuint		(double t, double totaltime, double max, double min);
	double InOutQuint	(double t, double totaltime, double max, double min);
	double InOutSine	(double t, double totaltime, double max, double min);
	double InExp		(double t, double totaltime, double max, double min);
	double OutExp		(double t, double totaltime, double max, double min);
	double InOutExp		(double t, double totaltime, double max, double min);
	double InCirc		(double t, double totaltime, double max, double min);
	double OutCirc		(double t, double totaltime, double max, double min);
	double InOutCirc	(double t, double totaltime, double max, double min);
	double InBack		(double t, double totaltime, double max, double min, double s);
	double OutBack		(double t, double totaltime, double max, double min, double s);
	double InOutBack	(double t, double totaltime, double max, double min, double s);
	double OutBounce	(double t, double totaltime, double max, double min);
	double InBounce		(double t, double totaltime, double max, double min);
	double InOutBounce	(double t, double totaltime, double max, double min);
	double Linear		(double t, double totaltime, double max, double min);
}