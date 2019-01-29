#include "NBS.h"


NBS::NBS() {
    printf("Empty NBS constructor\n");
}

NBS::NBS(double ***M, int A[2]) {
    //printf("a"); fflush(stdout);

    NumDecisionPointsActual = A[0];
	Cell pago[NumDecisionPointsMax][NumDecisionPointsMax];
	
	int i, j, m, n;
    
	for (i = 0; i < A[0]; i++) {
		for (j = 0; j < A[1]; j++) {
            pago[i][j].r1 = M[0][i][j];
            pago[i][j].r2 = M[1][i][j];
		}
	}

	double h1 = -10000000, h2 = -10000000, l1 = 10000000, l2 = 10000000;
	for (i = 0; i < NumDecisionPointsActual; i++) {
		for (j = 0; j < NumDecisionPointsActual; j++) {
			payoff1[i][j] = pago[i][j].r1;

			if (payoff1[i][j] > h1)
				h1 = payoff1[i][j];
			if (payoff1[i][j] < l1)
				l1 = payoff1[i][j];

			payoff2[j][i] = pago[i][j].r2;
			if (payoff2[j][i] > h2)
				h2 = payoff2[j][i];
			if (payoff2[j][i] < l2)
				l2 = payoff2[j][i];
		}
	}

	if (h1 == l1) {
		h1 ++;
		l1 --;
	}
	if (h2 == l2) {
		h2 ++;
		l2 --;
	}

    //printf("b"); fflush(stdout);

	GetDefend(alpha1, payoff2);
	GetDefend(alpha2, payoff1);
	GetAttack(delta1, payoff1);
	GetAttack(delta2, payoff2);
	GetGuarantee(delta1, alpha2, payoff1, &g1);
	GetGuarantee(delta2, alpha1, payoff2, &g2);

    //printf("c"); fflush(stdout);

//	printf("g (%.2lf, %.2lf)\n", g1, g2);

	for (i = 0; i < NumDecisionPointsActual; i++) {
		for (j = 0; j < NumDecisionPointsActual; j++) {
			A1[i][j] = payoff1[i][j] - g1;
			A2[i][j] = payoff2[i][j] - g2;
		}
	}

	double product, max = -1;
	Point p1, p2;
	Point tp1, tp2;
	int none = -1, ntwo = -1;
	int one, two;
	for (i = 0; i < NumDecisionPointsActual; i++) {
		for (j = 0; j < NumDecisionPointsActual; j++) {
			tp1.x = A1[i][j];
			tp1.y = A2[j][i];
			for (m = 0; m < NumDecisionPointsActual; m++) {
				for (n = 0; n < NumDecisionPointsActual; n++) {
					if ((i == m) && (j == n))
						continue;

					tp2.x = A1[m][n];
					tp2.y = A2[n][m];

					product = ComputeBargain(tp1, tp2, &one, &two);
					if (((tp1.x * tp1.y) > product) && (tp1.x > 0) && (tp1.y > 0)) {
						//printf("first\n");
						one = 1;
						two = 0;
						product = tp1.x * tp1.y;
					}
					else if (((tp2.x * tp2.y) > product) && (tp2.x > 0) && (tp2.y > 0)) {
						//printf("second\n");
						one = 0;
						two = 1;
						product = tp2.x * tp2.y;
					}
                    
                    //printf("product = %lf; max = %lf\n", product, max);

					if (product > max) {
						//printf("changing (from max = %lf) to product %lf: (one = %i, two = %i)\n", max, product, one, two);
						max = product;
						none = one;
						ntwo = two;
						p1.x = tp1.x;
						p1.y = tp1.y;
						p2.x = tp2.x;
						p2.y = tp2.y;
					}
				}
			}
		}
	}
    
    //printf("none = %i; ntwo = %i\n", none, ntwo);

    int c[2], a1[2], a2[2];
	for (i = 0; i < NumDecisionPointsActual; i++) {
		for (j = 0; j < NumDecisionPointsActual; j++) {
			if ((p1.x == A1[i][j]) && (p1.y == A2[j][i])) {
				//printf("%i: player1 takes action %i, and player2 takes action %i\n", none, i, j);
				c[0] = none;
				a1[0] = i;
				a2[0] = j;
			}
			if ((p2.x == A1[i][j]) && (p2.y == A2[j][i])) {
				//printf("%i: player1 takes action %i, and player2 takes action %i\n", ntwo, i, j);
				c[1] = ntwo;
				a1[1] = i;
				a2[1] = j;
			}
		}
	}
    
    //printf("d"); fflush(stdout);
    
    double w[2], pays1[2], pays2[2];
    w[0] = ((double)c[0]) / (c[0] + c[1]);
    w[1] = 1.0 - w[0];
    //printf("< %i %i %i %i %i %i > ", c[0], c[1], a1[0], a2[0], a1[1], a2[1]); fflush(stdout);
    pays1[0] = M[0][a1[0]][a2[0]];
    pays1[1] = M[0][a1[1]][a2[1]];
    pays2[0] = M[1][a1[0]][a2[0]];
    pays2[1] = M[1][a1[1]][a2[1]];
    
    //printf("f"); fflush(stdout);
    
    sol = new Solution(a1, a2, pays1, pays2, w);
    
    //printf("g"); fflush(stdout);
}

NBS::~NBS() {
    delete sol;
}

double NBS::ComputeBargain(Point tp1, Point tp2, int *one, int *two) {
	if (((tp1.x < 0) && (tp1.y < 0)) || ((tp2.x < 0) && (tp2.y < 0))) {
		return -1.0;
	}
    
    if ((2.0 * (tp1.y - tp2.y) * (tp1.x - tp2.x)) == 0) {
        //printf("caught a problem: (%lf, %lf), (%lf, %lf)\n", tp1.x, tp1.y, tp2.x, tp2.y);
        
        if ((tp1.x + tp1.y) > (tp2.x + tp2.y)) {
            *one = 1;
            *two = 0;
            return 0.0;
        }
        else {
            *one = 0;
            *two = 1;
            return 0.0;
        }
    }

	double wx = (-tp2.y * (tp1.x - tp2.x) - tp2.x * (tp1.y - tp2.y)) / (2.0 * (tp1.y - tp2.y) * (tp1.x - tp2.x));
//	printf("wx = %lf from points (%lf, %lf) and (%lf, %lf)\n", wx, tp1.x, tp1.y, tp2.x, tp2.y);

	if ((wx < 0) || (wx > 1)) {		
		if ((tp1.x * tp1.y) > (tp2.x * tp2.y)) {
			*one = 1;
			*two = 0;

			return (tp1.x * tp1.y);
		}
		else {
			*two = 1;
			*one = 0;

			return (tp2.x * tp2.y);
		}
	}
	else {
		int i;
		for (i = 1; i < 1000; i++) {
			if ((((wx * (double)i) - (int)((wx * (double)i) + 0.5)) < 0.01) && 
				((((1.0 - wx) * (double)i) - (int)(((1.0 - wx) * (double)i) + 0.5)) < 0.01)) {
				*one = (int)(wx * i + 0.5);
				*two = (int)((1.0 - wx) * i + 0.5);
				break;
			}
		}

		Point z;
		z.x = wx * tp1.x  + (1.0 - wx) * tp2.x;
		z.y = wx * tp1.y  + (1.0 - wx) * tp2.y;

//		printf("z1 = %lf, z2 = %lf\n", z.x, z.y);

		// fix to littman's algorithm?
		if ((z.x < 0) && (z.y < 0)) {
			//printf("censoring %.2lf, %.2lf\n", z.x, z.y);
			return -(z.x * z.y);
		}

		return (z.x * z.y);
	}


	return -1;
}

void NBS::GetGuarantee(double delta[NumDecisionPointsMax], double alpha[NumDecisionPointsMax],
					double payoff[NumDecisionPointsMax][NumDecisionPointsMax], double *g) {
	int i, j;

	*g = 0;

	for (i = 0; i < NumDecisionPointsActual; i++) {
		for (j = 0; j < NumDecisionPointsActual; j++) {
			*g += delta[i] * alpha[j] * payoff[i][j];
		}
	}
}

void NBS::GetDefend(double alpha[NumDecisionPointsMax], double payoff[NumDecisionPointsMax][NumDecisionPointsMax]) {
	double currentmin, tval, val;
	int j, k;
	double weights[NumDecisionPointsMax];

	for (j = 0; j < NumDecisionPointsActual; j++) {
		alpha[j] = 1.0 / (double)NumDecisionPointsActual;
		weights[j] = 0.0;
	}

	val = -1000.0;
	for (k = 0; k < NumDecisionPointsActual; k++) {
		tval = 0.0;
		for (j = 0; j < NumDecisionPointsActual; j++) {
			tval += alpha[j] * payoff[k][j];
		}
		if (tval > val)
			val = tval;
	}

	currentmin = val;


	RecurseMinimax(weights, alpha, &currentmin, payoff, 0);
}

void NBS::GetAttack(double *alpha, double payoff[NumDecisionPointsMax][NumDecisionPointsMax]) {
	double currentmax, val, tval;
	int j, k;
	double weights[NumDecisionPointsMax];

	for (j = 0; j < NumDecisionPointsActual; j++) {
		alpha[j] = 1.0 / (double)NumDecisionPointsActual;
		weights[j] = 0.0;
	}

	val = 1000000.0;
	for (k = 0; k < NumDecisionPointsActual; k++) {
		tval = 0.0;
		for (j = 0; j < NumDecisionPointsActual; j++) {
			tval += alpha[j] * payoff[j][k];
		}
		if (tval < val)
			val = tval;
	}

	currentmax = -1000000; //val;

	RecurseMaximin(weights, alpha, &currentmax, payoff, 0);
/*	
	for (i = 0.0; i <= 1.00001; i += 0.01) {
		tval = val = 0.0;

		val = i * payoff[0][0] + (1.0 - i) * payoff[0][1];
		tval = i * payoff[1][0] + (1.0 - i) * payoff[1][1];

		if (tval < val)
			val = tval;

		if (val > currentmax) {
			currentmax = val;
			alpha[0] = i;
			alpha[1] = (1.0 - i);
		}
	}
*/
}

void NBS::RecurseMaximin(double *weights, double *alpha, double *currentmax, double payoff[NumDecisionPointsMax][NumDecisionPointsMax], int index) {
	if (index == (NumDecisionPointsActual - 1)) {
		double tval, val;
		double weights2[NumDecisionPointsMax];
		int k, j;

		for (weights[index] = 0.0; weights[index] <= 1.00001; weights[index] += 0.1) {
			if (Normalize(weights, weights2)) {
				//fprintf(outfile, "%.2lf %.2lf %.2lf %.2lf\n", weights2[0], weights2[1], weights2[2], weights2[3]);
				val = 1000000;
				for (k = 0; k < NumDecisionPointsActual; k++) {
					tval = 0.0;
					for (j = 0; j < NumDecisionPointsActual; j++) {
						tval += weights2[j] * payoff[j][k];
					}
					if (tval < val)
						val = tval;
				}

				if (val > *currentmax) {
					*currentmax = val;
					for (k = 0; k < NumDecisionPointsActual; k++)
						alpha[k] = weights2[k];
				}
			}
		}

		return;
	}

	else {
		for (weights[index] = 0.0; weights[index] <= 1.00001; weights[index] += 0.1)
			RecurseMaximin(weights, alpha, currentmax, payoff, index + 1);
	}
//	if (weights[index] < 0.991)
//		weights[index] += 0.01;
//	RecurseMaximin(weights, alpha, currentmax, payoff, index + 1);
}

void NBS::RecurseMinimax(double *weights, double *alpha, double *currentmin, double payoff[NumDecisionPointsMax][NumDecisionPointsMax], int index) {
	if (index == (NumDecisionPointsActual - 1)) {
		//printf("hit the end\n");
		double tval, val;
		double weights2[NumDecisionPointsMax];
		int k, j;

		for (weights[index] = 0.0; weights[index] <= 1.00001; weights[index] += 0.1) {
			if (Normalize(weights, weights2)) {
				val = -1000;
				for (k = 0; k < NumDecisionPointsActual; k++) {
					tval = 0.0;
					for (j = 0; j < NumDecisionPointsActual; j++) {
						tval += weights2[j] * payoff[k][j];
					}
					if (tval > val)
						val = tval;
				}

				if (val < *currentmin) {
					*currentmin = val;
					for (k = 0; k < NumDecisionPointsActual; k++)
						alpha[k] = weights2[k];
				}
			}
		}

		return;
	}
	else {
		for (weights[index] = 0.0; weights[index] <= 1.00001; weights[index] += 0.1)
			RecurseMinimax(weights, alpha, currentmin, payoff, index + 1);
	}
//	if (weights[index] < 0.991)
//		weights[index] += 0.01;
//	RecurseMinimax(weights, alpha, currentmin, payoff, index + 1);
}

int NBS::Normalize(double *m, double *n) {
	double mag = 0.0;
	int i;

	for (i = 0; i < NumDecisionPointsActual; i++)
		mag += m[i];

	if (mag == 0.0)
		return 0;
	else {
		for (i = 0; i < NumDecisionPointsActual; i++)
			n[i] = m[i] / mag;
	}

	return 1;
}
