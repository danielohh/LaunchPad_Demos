///
/// @mainpage	LCD_Joystick
///
/// @details	Joystick controlled cursor
/// @n
/// @n @a		Developed with [embedXcode+](http://embedXcode.weebly.com)
///
/// @author		Rei Vilo
/// @author		http://embeddedcomputing.weebly.com
/// @date		11/12/2013 10:19
/// @version	101
///
/// @copyright	(c) Rei Vilo, 2013
/// @copyright	CC = BY SA NC
///
/// @see		ReadMe.txt for references
///


///
/// @file		LCD_Joystick.ino
/// @brief		Main sketch
///
/// @details	Joystick controlled cursor
/// @n @a		Developed with [embedXcode+](http://embedXcode.weebly.com)
///
/// @author		Rei Vilo
/// @author		http://embeddedcomputing.weebly.com
/// @date		11/12/2013 10:19
/// @version	101
///
/// @copyright	(c) Rei Vilo, 2013
/// @copyright	CC = BY SA NC
///
/// @see		ReadMe.txt for references
/// @n
///


// Core library for code-sense
#if defined(ENERGIA) // LaunchPad MSP430, Stellaris and Tiva, Experimeter Board FR5739 specific
#include "Energia.h"
#else // error
#error Platform not defined
#endif

// Prototypes


// Include application, user and local libraries
#include "SPI.h"
#include "Screen_HX8353E.h"
Screen_HX8353E myScreen;




#define N_WAVE      256    /* full length of Sinewave[] */
#define LOG2_N_WAVE 8      /* log2(N_WAVE) */


//#include <stdint.h>
//#include <stdlib.h>

const int8_t Sinewave[N_WAVE-N_WAVE/4] = {
0, 3, 6, 9, 12, 15, 18, 21, 
24, 28, 31, 34, 37, 40, 43, 46, 
48, 51, 54, 57, 60, 63, 65, 68, 
71, 73, 76, 78, 81, 83, 85, 88, 
90, 92, 94, 96, 98, 100, 102, 104, 
106, 108, 109, 111, 112, 114, 115, 117, 
118, 119, 120, 121, 122, 123, 124, 124, 
125, 126, 126, 127, 127, 127, 127, 127, 

127, 127, 127, 127, 127, 127, 126, 126, 
125, 124, 124, 123, 122, 121, 120, 119, 
118, 117, 115, 114, 112, 111, 109, 108, 
106, 104, 102, 100, 98, 96, 94, 92, 
90, 88, 85, 83, 81, 78, 76, 73, 
71, 68, 65, 63, 60, 57, 54, 51, 
48, 46, 43, 40, 37, 34, 31, 28, 
24, 21, 18, 15, 12, 9, 6, 3, 

0, -3, -6, -9, -12, -15, -18, -21, 
-24, -28, -31, -34, -37, -40, -43, -46, 
-48, -51, -54, -57, -60, -63, -65, -68, 
-71, -73, -76, -78, -81, -83, -85, -88, 
-90, -92, -94, -96, -98, -100, -102, -104, 
-106, -108, -109, -111, -112, -114, -115, -117, 
-118, -119, -120, -121, -122, -123, -124, -124, 
-125, -126, -126, -127, -127, -127, -127, -127, 

/*-127, -127, -127, -127, -127, -127, -126, -126, 
-125, -124, -124, -123, -122, -121, -120, -119, 
-118, -117, -115, -114, -112, -111, -109, -108, 
-106, -104, -102, -100, -98, -96, -94, -92, 
-90, -88, -85, -83, -81, -78, -76, -73, 
-71, -68, -65, -63, -60, -57, -54, -51, 
-48, -46, -43, -40, -37, -34, -31, -28, 
-24, -21, -18, -15, -12, -9, -6, -3, */
};

inline int8_t FIX_MPY(int8_t a, int8_t b)
{
  
    /* shift right one less bit (i.e. 15-1) */
    int16_t c = ((int16_t)a * (int16_t)b) >> 6;
    /* last bit shifted out = rounding-bit */
    b = c & 0x01;
    /* last shift + rounding bit */
    a = (c >> 1) + b;

        /*
        Serial.print16_tln(Sinewave[3]);
        Serial.print16_tln(c);
        Serial.print16_tln(a);
        while(1);*/

    return a;
}

/*
  fix_fft() - perform forward/inverse fast Fourier transform.
  fr[n],fi[n] are real and imaginary arrays, both INPUT AND
  RESULT (in-place FFT), with 0 <= n < 2**m; set inverse to
  0 for forward transform (FFT), or 1 for iFFT.
*/
int16_t fix_fft(int8_t fr[], int8_t fi[], int16_t m, int16_t inverse)
{
    int16_t mr, nn, i, j, l, k, istep, n, scale, shift;
    int8_t qr, qi, tr, ti, wr, wi;

    n = 1 << m;

    /* max FFT size = N_WAVE */
    if (n > N_WAVE)
        return -1;

    mr = 0;
    nn = n - 1;
    scale = 0;

    /* decimation in time - re-order data */
    for (m=1; m<=nn; ++m) {
        l = n;
        do {
            l >>= 1;
        } while (mr+l > nn);
        mr = (mr & (l-1)) + l;

        if (mr <= m)
            continue;
        tr = fr[m];
        fr[m] = fr[mr];
        fr[mr] = tr;
        ti = fi[m];
        fi[m] = fi[mr];
        fi[mr] = ti;
    }

    l = 1;
    k = LOG2_N_WAVE-1;
    while (l < n) {
        if (inverse) {
            /* variable scaling, depending upon data */
            shift = 0;
            for (i=0; i<n; ++i) {
                j = fr[i];
                if (j < 0)
                    j = -j;
                m = fi[i];
                if (m < 0)
                    m = -m;
                if (j > 16383 || m > 16383) {
                    shift = 1;
                    break;
                }
            }
            if (shift)
                ++scale;
        } else {
            /*
              fixed scaling, for proper normalization --
              there will be log2(n) passes, so this results
              in an overall factor of 1/n, distributed to
              maximize arithmetic accuracy.
            */
            shift = 1;
        }
        /*
          it may not be obvious, but the shift will be
          performed on each data point16_t exactly once,
          during this pass.
        */
        istep = l << 1;
        for (m=0; m<l; ++m) {
            j = m << k;
            /* 0 <= j < N_WAVE/2 */
            //wr =  pgm_read_word_near(Sinewave + j+N_WAVE/4);
            wr =  Sinewave[j+N_WAVE/4];

            //wi = -pgm_read_word_near(Sinewave + j);
            wi = -Sinewave[j];
            if (inverse)
                wi = -wi;
            if (shift) {
                wr >>= 1;
                wi >>= 1;
            }
            for (i=m; i<n; i+=istep) {
                j = i + l;
                tr = FIX_MPY(wr,fr[j]) - FIX_MPY(wi,fi[j]);
                ti = FIX_MPY(wr,fi[j]) + FIX_MPY(wi,fr[j]);
                qr = fr[i];
                qi = fi[i];
                if (shift) {
                    qr >>= 1;
                    qi >>= 1;
                }
                fr[j] = qr - tr;
                fi[j] = qi - ti;
                fr[i] = qr + tr;
                fi[i] = qi + ti;
            }
        }
        --k;
        l = istep;
    }
    return scale;
}

/*
  fix_fftr() - forward/inverse FFT on array of real numbers.
  Real FFT/iFFT using half-size complex FFT by distributing
  even/odd samples int16_to real/imaginary arrays respectively.
  In order to save data space (i.e. to avoid two arrays, one
  for real, one for imaginary samples), we proceed in the
  following two steps: a) samples are rearranged in the real
  array so that all even samples are in places 0-(N/2-1) and
  all imaginary samples in places (N/2)-(N-1), and b) fix_fft
  is called with fr and fi point16_ting to index 0 and index N/2
  respectively in the original array. The above guarantees
  that fix_fft "sees" consecutive real samples as alternating
  real and imaginary samples in the complex array.
*/
int16_t fix_fftr(int8_t f[], int16_t m, int16_t inverse)
{
    int16_t i, NN = 1<<(m-1), scale = 0;
    int8_t tt, *fr=f, *fi=&f[N];

    if (inverse)
        scale = fix_fft(fi, fr, m-1, inverse);
    for (i=1; i<NN; i+=2) {
        tt = f[NN+i-1];
        f[NN+i-1] = f[i];
        f[i] = tt;
    }
    if (! inverse)
        scale = fix_fft(fi, fr, m-1, inverse);
    return scale;
}

#define log2FFT   5
#define FFT_SIZE  (1<<log2FFT)
#define Nx        (2 * FFT_SIZE)
#define log2N     (log2FFT + 1)
#define BAND_FREQ_KHZ	8

#define MIC 6

#define OFFSET 2020
#define RANGE	128
uint8_t col[FFT_SIZE], col_p[FFT_SIZE];
uint16_t colour;
uint32_t z;

int8_t data[Nx], im[Nx];

// Add setup code
void setup()
{

    myScreen.begin();
	uint8_t i=0;
	for (i=0;i<FFT_SIZE;i++) col[i] = 1;
    
}

uint8_t cnt=0;

void loop()
{
	uint8_t i=0;

	for (i=0;i<Nx;i++) {

		// time delay between adc samples
		// this will become the band frequency after time - frequency conversion
		
		//TA0CCR0 += (16000/(BAND_FREQ_KHZ*2))-1;	// begin counting for next period
		// 25000/BAND_FREQ_KHZ;

		uint16_t start = micros();
		uint16_t adc = map(analogRead(MIC), OFFSET-RANGE, OFFSET+RANGE, 0, 127);
		data[i] = adc;
		//data[i] = (ADC10MEM>>2) - 128;		// signal leveling?
		//data[i] = ADC10MEM - 512;		// signal leveling?
		im[i] = 0;

		while ((micros() - start) < 68) __asm("nop");

	}//for

	fix_fft(data, im, log2N, 0);	// thank you, Tom Roberts(89),Malcolm Slaney(94),...
	colour = greenColour;

	for (i=1;i<FFT_SIZE;i++) {
		//col[i] = 60 + (cnt%16);
		data[i] = sqrt(data[i]*data[i] + im[i]*im[i]);
		if (col[i] > (data[i]+1))
			col[i] -= 2;
		else
			col[i] = data[i];

		if (col[i] != col_p[i]) {
			myScreen.dRectangle((i*4)+1, 100-col_p[i], 3, 3, blackColour);
			myScreen.dRectangle((i*4)+1, 100-col[i], 3, 3, colour);
			col_p[i] = col[i];
		}//if
	}//for

	cnt++;

}

