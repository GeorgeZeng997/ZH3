#ifndef __ADC_H
#define __ADC_H

#define ADC_CONVERT_LENGTH 64


void adc_dma_config(void);
void adc_task(void *p);
void adc_dma_callback(void);
#endif
