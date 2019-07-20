#include "Mathematics.h"

void mathematics::convertMatrix(Matrix4f & matrix, Quaternion & quaternion, Vector3f & vector)
{

	float sq[4];
	// 11 =0 22=5 33=10
	sq[0] = matrix[0][0] + matrix[1][1] + matrix[2][2];
	sq[1] = matrix[0][0] - matrix[1][1] - matrix[2][2];
	sq[2] = -matrix[0][0] + matrix[1][1] - matrix[2][2];
	sq[3] = -matrix[0][0] - matrix[1][1] + matrix[2][2];
	int gIndex = -1;	float g = -99999;
	for (int i = 0; i < 4; i++)
	{
		if (sq[i] > g)
		{
			g = sq[i];
			gIndex = i;
		}
	}
	float w, x, y, z;
	switch (gIndex)
	{
	case 0:	// w greatest
		w = std::sqrt(sq[0] + 1) / 2;
		x = (matrix[1][2] - matrix[2][1]) / (4 * w);
		y = (matrix[2][0] - matrix[0][2]) / (4 * w);
		z = (matrix[0][1] - matrix[1][0]) / (4 * w);
		break;
	case 1: // x greatest
		x = std::sqrt(sq[1] + 1) / 2;
		w = (matrix[1][2] - matrix[2][1]) / (4 * x);
		y = (matrix[0][1] + matrix[1][0]) / (4 * x);
		z = (matrix[0][2] + matrix[2][0]) / (4 * x);
		break;
	case 2:	// y greatest
		y = std::sqrt(sq[2] + 1) / 2;
		w = (matrix[2][0] - matrix[0][2]) / (4 * y);
		x = (matrix[1][0] + matrix[0][1]) / (4 * y);
		z = (matrix[2][1] + matrix[1][2]) / (4 * y);
		break;
	case 3:
		z = std::sqrt(sq[3] + 1) / 2;
		w = (matrix[0][1] - matrix[1][0]) / (4 * z);
		x = (matrix[2][0] + matrix[0][2]) / (4 * z);
		y = (matrix[2][1] + matrix[1][2]) / (4 * z);
		break;
	}


	quaternion = Quaternion(w, x, y, z);
	vector.x(matrix[3][0]);	vector.y(matrix[3][1]);	vector.z(matrix[3][2]);
}
