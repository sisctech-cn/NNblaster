/**
 ******************************************************************************
 * file    ai_net_layer.h
 * author  sunlingge
 * version V100
 * brief   the ai net layer driver.
 ******************************************************************************
 */
#ifndef AI_NET_LAYER_H
#define AI_NET_LAYER_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/
#if (AI_PRODUCT_OPTION_NET_IMAGE == AI_NET_IMAGE_416_416) 
#define AI_NET_LAYER_RO_MEM_LENGTH_MAX			(237*1024*1024)
#define AI_NET_LAYER_RW0_MEM_LENGTH_MAX			(85*1024*1024)
#define AI_NET_LAYER_RW1_MEM_LENGTH_MAX			(85*1024*1024)
#endif
#if (AI_PRODUCT_OPTION_NET_IMAGE == AI_NET_IMAGE_608_608) 
#define AI_NET_LAYER_RO_MEM_LENGTH_MAX			(237*1024*1024)
#define AI_NET_LAYER_RW0_MEM_LENGTH_MAX			(181*1024*1024)
#define AI_NET_LAYER_RW1_MEM_LENGTH_MAX			(181*1024*1024)
#endif

/*------------------------- Macro Definition --------------------------------*/
typedef struct ai_net_layer_data_struct {
    INT32_T layer_index;
	UINT32_T batch;
    UINT32_T inputs;
    UINT32_T h;
    UINT32_T w;
    UINT32_T c;
	UINT32_T adam;
	FLOAT_AI_T b1;
	FLOAT_AI_T b2;
	FLOAT_AI_T eps;
	UINT32_T out_h;
	UINT32_T out_w;
	UINT32_T out_c;
	UINT32_T outputs;
	UINT32_T bit_align;
	UINT32_T truths;
	UINT32_T reverse;
	UINT32_T lda_align;
	UINT32_T n;
	UINT32_T workspace_size;
	FLOAT_AI_T bflops;
	AI_NET_LAYER_CONTENT weights;
	AI_NET_LAYER_CONTENT weights_update;
	AI_NET_LAYER_CONTENT biases;
	AI_NET_LAYER_CONTENT biases_update;
	AI_NET_LAYER_CONTENT output;
	AI_NET_LAYER_CONTENT delta;
	AI_NET_LAYER_CONTENT binary_weights;
	AI_NET_LAYER_CONTENT cweights;
	AI_NET_LAYER_CONTENT scales;
	AI_NET_LAYER_CONTENT binary_input;
	AI_NET_LAYER_CONTENT mean_arr;
	AI_NET_LAYER_CONTENT scale_updates;
	AI_NET_LAYER_CONTENT mean;
	AI_NET_LAYER_CONTENT variance;
	AI_NET_LAYER_CONTENT mean_delta;
	AI_NET_LAYER_CONTENT variance_delta;
    AI_NET_LAYER_CONTENT rolling_mean;
    AI_NET_LAYER_CONTENT rolling_variance;
    AI_NET_LAYER_CONTENT x;
    AI_NET_LAYER_CONTENT x_norm;
	AI_NET_LAYER_CONTENT m;
	AI_NET_LAYER_CONTENT v;
	AI_NET_LAYER_CONTENT cost;
	AI_NET_CONFIG_SECTION *layer_config;
	/* cmodel 1*/
	UINT32_T h_align;
	UINT32_T w_align;
	UINT32_T c_align;
	UINT32_T inputs_align;
	UINT32_T out_h_align;
	UINT32_T out_w_align;
	UINT32_T out_c_align;
	UINT32_T outputs_align;
	UINT32_T padding;
	UINT32_T padding_next;
} AI_NET_LAYER_DATA, *AI_NET_LAYER_DATA_PTR;

typedef struct ai_net_layer_func_struct {
	ERROR_T (*forward)	(AI_NET_STATE *net_state, UINT32_T layer_index);
    ERROR_T (*backward)	(AI_NET_STATE *net_state, UINT32_T layer_index);
    ERROR_T (*update)	(AI_NET_STATE *net_state, UINT32_T layer_index);
} AI_NET_LAYER_FUNC, *AI_NET_LAYER_FUNC_PTR;

typedef struct ai_net_layer_report_struct {
	UINT32_T ro_mem_max;
	UINT32_T ro_mem_total;
	UINT32_T rw0_mem_max;
	UINT32_T rw0_mem_total;
	UINT32_T rw1_mem_max;
	UINT32_T rw1_mem_total;
	UINT32_T workspace_size_max;
	UINT32_T max_inputs;
	UINT32_T max_outputs;
	FLOAT32_T bflops_total;
} AI_NET_LAYER_REPORT, *AI_NET_LAYER_REPORT_PTR;

typedef struct ai_net_layer_struct {
	INT32_T layer_number;
	AI_NET_LAYER_DATA layer_data[AI_NET_LAYER_MAX];
	AI_NET_LAYER_FUNC layer_func[AI_NET_LAYER_MAX];
	AI_NET_LAYER_BUFFER layer_ro;
	AI_NET_LAYER_BUFFER layer_rw0;
	AI_NET_LAYER_BUFFER layer_rw1;
	UINT8_T layer_ro_buffer[AI_NET_LAYER_RO_MEM_LENGTH_MAX];
	UINT8_T layer_rw0_buffer[AI_NET_LAYER_RW0_MEM_LENGTH_MAX];
	UINT8_T layer_rw1_buffer[AI_NET_LAYER_RW1_MEM_LENGTH_MAX];
	AI_NET_LAYER_REPORT layer_report;
} AI_NET_LAYER, *AI_NET_LAYER_PTR;

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiNetLayerInit(AI_NET_LAYER *net_layer, AI_NET_CONFIG *net_config);
ERROR_T AiNetLayerDefault(AI_NET_CONFIG *net_config, UINT32_T section_index);
ERROR_T AiNetLayerSetNetwork(AI_NET_NETWORK *net_network, AI_NET_LAYER *net_layer, UINT32_T layer_index);
ERROR_T AiNetLayerReportInit(AI_NET_LAYER *net_layer);
ERROR_T AiNetLayerReportSet(AI_NET_LAYER *net_layer, AI_NET_NETWORK *net_network); 

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

