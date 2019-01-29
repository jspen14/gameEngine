#include "minimaxLog.h"

minimaxLog::minimaxLog() {
}

minimaxLog::minimaxLog(int _nActions[2], int _me) {//, int _numStates) {
	ms = new double[_nActions[_me]];
	mpi = new double[_nActions[_me]];
	payoffs = new double[_nActions[0] * _nActions[1]];

	mv = lowmv = -abs(SENTINAL);
	nott = new bool[_nActions[_me]];

	int i;
	for (i = 0; i < _nActions[_me]; i++)
		ms[i] = 1.0 / (double)_nActions[_me];
}

minimaxLog::~minimaxLog() {
	delete ms;
	delete mpi;
	delete payoffs;
	delete nott;
}

void minimaxLog::getMinimax(int *nActions, int me, double **_M) {
	int c = 0, i, j;
	for (i = 0; i < nActions[0]; i++) {
		for (j = 0; j < nActions[1]; j++) {
			payoffs[c] = _M[i][j];
			c ++;
		}
	}
	
    getMinimax(nActions, me, payoffs);
}

void minimaxLog::getAttack(int *nActions, int me, double **_M) {
	int c = 0, i, j;
	for (i = 0; i < nActions[0]; i++) {
		for (j = 0; j < nActions[1]; j++) {
			payoffs[c] = -_M[i][j];
			c ++;
		}
	}
	
    getMinimax(nActions, me, payoffs);
    mv = -mv;
}

void minimaxLog::getMinimax(int *nActions, int me, double *payoff) {
	// first check for a pure strategy solution
	if (pureStrategy(mpi, nActions, me, payoff)) {
		//printf("pure strategy solution\n");
		mv = evaluatePolicy(nActions, mpi, payoff, me);
		int i;
        double sum = 0.0;
		for (i = 0; i < nActions[me]; i++) {
			ms[i] = mpi[i];
            sum += ms[i];
        }
        if (sum < 0.9999) {
            printf("maximin problem (%lf): ", sum);
            for (i = 0; i < nActions[me]; i++) {
                printf("%.6lf ", ms[i]);
            }
            printf("\n");
            
            exit(1);
        }
	}
	else {
		mv = -abs(SENTINAL);
		policyIterations(mpi, nActions, me, payoff, 1.0, 0, 0.0, 1.0 / (double)(nActions[me]), 1.0);

		int i;
        double sum = 0.0;
		for (i = 0; i < nActions[me]; i++) {
            sum += ms[i];
        }
        if (sum < 0.9999) {
            printf("maximin problem (%lf): ", sum);
            for (i = 0; i < nActions[me]; i++) {
                printf("%.6lf ", ms[i]);
            }
            printf("\n");
            
            exit(1);
        }
	}
    
    /*if (nActions[me] > 5) {
        printf("mv = %.3lf\n", mv);
        int i;
        for (i = 0; i < nActions[me]; i++)
            printf("%lf ", ms[i]);
        printf("\n");
    }*/
}

double minimaxLog::policyIterations(double *mpi, int *nActions, int me, double *payoff, double remaining, int index, double l, double m, double r) {
	int i;
    
    //for (i = 0; i < nActions[me]; i++)
    //    printf("%.2lf ", mpi[i]);
    //printf("\n");
    /*if ((nActions[0] > 4) || (nActions[1] > 4)) {
        for (i = 0; i < nActions[me]; i++)
            mpi[i] = ms[i] = 1.0 / nActions[me];
        mv = getDimmensionHigh(mpi, nActions, me, payoff, index, remaining, l, m, r);
        return mv;
    }*/
	
	if ((nActions[me] - index) == 2) {
		double tmp = getDimmensionHigh(mpi, nActions, me, payoff, index, remaining, l, m, r);
		if (tmp > mv) {
			mv = tmp;
			//printf("1st new mv = %lf; (%lf, %lf)\n", mv, mpi[0], mpi[1]);
			for (i = 0; i < nActions[me]; i++)
				ms[i] = mpi[i];
		}
		return tmp;
	}
	else if (remaining <= 0.0) {
		for (i = index; i < nActions[me]; i++)
			mpi[i] = 0.0;

		double tmp = evaluatePolicy(nActions, mpi, payoff, me);
		if (tmp > mv) {
			mv = tmp;
			//printf("2nd new mv = %lf; (%lf, %lf)\n", mv, mpi[0], mpi[1]);			
			for (i = 0; i < nActions[me]; i++)
				ms[i] = mpi[i];
		}
		return tmp;
	}

	double Val[3], right;

	mpi[index] = l; //(remaining - l);
	right = remaining - mpi[index];
	Val[0] = policyIterations(mpi, nActions, me, payoff, remaining - mpi[index], index+1, 
								0.0, right / (double)(nActions[me] - (index + 1)), right);

	mpi[index] = m; //(remaining - m);
	right = remaining - mpi[index];
	Val[1] = policyIterations(mpi, nActions, me, payoff, remaining - mpi[index], index+1, 
								0.0, right / (double)(nActions[me] - (index + 1)), right);

	mpi[index] = r; //(remaining - r);
	right = remaining - mpi[index];
	Val[2] = policyIterations(mpi, nActions, me, payoff, remaining - mpi[index], index+1, 
								0.0, right / (double)(nActions[me] - (index + 1)), right);

	int order[3];
	getOrder(Val[0], Val[1], Val[2], order);

	//if ((r - l) < GRANULARITY) {
    	if (((r - l) < GRANULARITY) || ((nActions[me] > 4) && ((r-l) < GRANULARITY2))) {
		return Val[order[0]];
	}

	if (order[0] == 0) {
		return policyIterations(mpi, nActions, me, payoff, remaining, index, 
									l, (m-l) / (double)(nActions[me] - index) + l, m);
	}
	else if (order[0] == 2) {
		return policyIterations(mpi, nActions, me, payoff, remaining, index, 
									m, (r-m) / (double)(nActions[me] - index) + m, r);
	}
	else {
		// check the quarter points
		double lq, rq;
		
		mpi[index] = ((m - l) / (double)(nActions[me] - (index+1))) + l; //(remaining - ((m+l) / 2.0));
		right = remaining - mpi[index];
		lq = policyIterations(mpi, nActions, me, payoff, remaining - mpi[index], index+1, 
								0.0, right / (double)(nActions[me] - (index + 1)), right);

		if (lq > Val[1]) {
			return policyIterations(mpi, nActions, me, payoff, remaining, index, 
								l, (m-l) / (double)(nActions[me] - index) + l, m);
		}

		mpi[index] = ((r-m) / (double)(nActions[me] - (index+1))) + m; //(remaining - ((m+r) / 2.0));
		right = remaining - mpi[index];
		rq = policyIterations(mpi, nActions, me, payoff, remaining - mpi[index], index+1, 
								0.0, right / (double)(nActions[me] - (index + 1)), right);

		if (rq > Val[1]) {
			return policyIterations(mpi, nActions, me, payoff, remaining, index, 
										m, (r-m) / (double)(nActions[me] - index) + m, r);
		}

		return policyIterations(mpi, nActions, me, payoff, remaining, index, 
									(m-l) / (double)(nActions[me] - index) + l, m, 
									(r-m) / (double)(nActions[me] - index) + m);
	}	
}

double minimaxLog::getDimmensionHigh(double *mpi, int *nActions, int me, double *payoff, int index, 
									double remaining, double l, double m, double r) {
	double Val[3];

	mpi[index] = (remaining - l);
	mpi[index + 1] = remaining - mpi[index];
	Val[0] = evaluatePolicy(nActions, mpi, payoff, me);

	mpi[index] = (remaining - m);
	mpi[index + 1] = remaining - mpi[index];
	Val[1] = evaluatePolicy(nActions, mpi, payoff, me);

	mpi[index] = (remaining - r);
	mpi[index + 1] = remaining - mpi[index];
	Val[2] = evaluatePolicy(nActions, mpi, payoff, me);

	int order[3];
	getOrder(Val[0], Val[1], Val[2], order);
	
	//printf("%i (%lf); %i (%lf); %i (%lf)\n", order[0], Val[order[0]], order[1], Val[order[1]], order[2], Val[order[2]]);

	if (((r - l) < GRANULARITY) || ((nActions[me] > 4) && ((r-l) < GRANULARITY2))) {
		if (order[0] == 0) {
			mpi[index] = (remaining - l);
			mpi[index + 1] = remaining - mpi[index];
		}
		else if (order[0] == 1) {
			mpi[index] = (remaining - m);
			mpi[index + 1] = remaining - mpi[index];
		}
		
		return Val[order[0]];
	}

	if (order[0] == 0) {
		return getDimmensionHigh(mpi, nActions, me, payoff, index, remaining, l, (m+l) / 2.0, m);
	}
	else if (order[0] == 2) {
		return getDimmensionHigh(mpi, nActions, me, payoff, index, remaining, m, (m+r)/2.0, r);
	}
	else {
		// check the quarter points
		double lq, rq;

		mpi[index] = (remaining - ((m+l) / 2.0));
		mpi[index + 1] = remaining - mpi[index];
		lq = evaluatePolicy(nActions, mpi, payoff, me);

		if (lq > Val[1]) {
			return getDimmensionHigh(mpi, nActions, me, payoff, index, remaining, l, (m+l) / 2.0, m);
		}

		mpi[index] = (remaining - ((m+r) / 2.0));
		mpi[index + 1] = remaining - mpi[index];
		rq = evaluatePolicy(nActions, mpi, payoff, me);

		if (rq > Val[1]) {
			return getDimmensionHigh(mpi, nActions, me, payoff, index, remaining, m, (m+r) / 2.0, r);
		}

		return getDimmensionHigh(mpi, nActions, me, payoff, index, remaining, (l+m)/2.0, m, (r+m)/2.0);
	}
}

void minimaxLog::getOrder(double lVal, double mVal, double rVal, int *order) {
	if (lVal > mVal) {
		if (lVal > rVal) {
			order[0] = 0;
			if (rVal > mVal) {
				order[1] = 2;
				order[2] = 1;
			}
			else {
				order[1] = 1;
				order[2] = 2;
			}
		}
		else {
			order[0] = 2;
			order[1] = 0;
			order[2] = 1;
		}
	}
	else {
		if (mVal > rVal) {
			order[0] = 1;
			if (rVal > lVal) {
				order[1] = 2;
				order[2] = 0;
			}
			else {
				order[1] = 0;
				order[2] = 2;
			}
		}
		else {
			order[0] = 2;
			order[1] = 1;
			order[2] = 0;
		}
	}
}

double minimaxLog::evaluatePolicy(int *nActions, double *mpi, double *payoff, int me) {
	int i, j, s;
	double V;
	double min = abs(SENTINAL);
	
	//printf("policy: %lf %lf;  ", mpi[0], mpi[1]);

	for (i = 0; i < nActions[1-me]; i++) {
		V = 0.0;
		for (j = 0; j < nActions[me]; j++) {
			if (me == 1)
				s = i * nActions[1] + j;
			else
				s = j * nActions[1] + i;

			if (payoff[s] == -abs(SENTINAL)) {
				printf("something is wrong\n");
				V += 999999.0 * mpi[j];  // changes this eventually ?
			}
			else {
	//			printf("%.2lf + ", payoff[s] * mpi[j]);
				
				V += payoff[s] * mpi[j];
			}
		}

		if (V < min)
			min = V;
	}
	
	//printf("  min = %lf\n", min);

	return min;
}

//
//	Stuff for pure strategy checks;
//
bool minimaxLog::highest(int index, int *nActions, double *payoff, int me, bool *nott) {
	int i;
	double altito;
	int s;

	if (me == 0)
		s = index;
	else
		s = index * nActions[1];
	
	//printf("s = %i\n", s);

	altito = payoff[s];
	//printf("%lf ", payoff[s]); 
	for (i = 1; i < nActions[me]; i++) {
		if (me == 0)
			s = i * nActions[1] + index;
		else
			s = index * nActions[1] + i;

		//printf("%lf ", payoff[s]);		
		if (payoff[s] > altito)
			altito = payoff[s];
	}

	bool rval = false;
	for (i = 0; i < nActions[me]; i++) {
		if (me == 0)
			s = i * nActions[1] + index;
		else
			s = index * nActions[1] + i;

		if (payoff[s] < altito)
			nott[i] = true;
		else if (!nott[i])
			rval = true;
	}

	//printf("\nnott (%i): ", index);
	//for (i = 0; i < nActions[me]; i++)
	//	printf("%i ", (int)(nott[i]));
	//printf("\n");
	
	return rval;
}

bool minimaxLog::pureStrategy(double *mpi, int *nActions, int me, double *payoff) {
	int i;
	for (i = 0; i < nActions[me]; i++)
		nott[i] = false;

	highest(0, nActions, payoff, me, nott);
	for (i = 1; i < nActions[1-me]; i++) {
		if (!highest(i, nActions, payoff, me, nott))
			return false;
	}

	for (i = 0; i < nActions[me]; i++) {
		mpi[i] = 0.0;		
	}
	double sum = 0.0;
	for (i = 0; i < nActions[me]; i++) {
		if (!nott[i]) {
			mpi[i] = 1.0;
			sum ++;
		}
	}
	
	for (i = 0; i < nActions[me]; i++) {
		mpi[i] /= sum;
	}

	return true;
}

