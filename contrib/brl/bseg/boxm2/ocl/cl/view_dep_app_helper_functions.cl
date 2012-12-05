//The BOXM2_MOG6_view datatype is a float16 and is laid out as follows:
// [mu_x, sigma_x, mu_y, sigma_y, mu_z, sigma_z, mu_(-x), sigma_(-x), mu_(-y), sigma_(-y), mu_(-z), sigma_(-z)]

//The BOXM2_NUM_OBS_VIEW datatype is a float8 and is laid out as follows:
// [num_obs_(x),num_obs_(y),num_obs_(z),num_obs_(-x),num_obs_(-y),num_obs_(-z) ]
// num_obs_(x) is simply the sum of weights received so far for the appearance model at direction x.
// The weights are computed as visibility times the dot product between the viewing direction and appearance model direction. 

//DECLARE app_model_view_directions

__constant  float4  app_model_view_directions[8] = {  (float4)(0,       0,      1, 0),
                                                      (float4)(0.707,    0,      0.707,0),
                                                      (float4)(0.354,    0.612,  0.707, 0),
                                                      (float4)(-0.354,    0.612, 0.707,0),
                                                      (float4)(-0.707,    0,     0.707,0),
                                                      (float4)(-0.354,   -0.612, 0.707,0),
                                                      (float4)(0.354,    -0.612, 0.707,0),
                                                      (float4)(0,         0,     0,0)}; 

/*
__constant  float4  app_model_view_directions[8] = {  (float4)(0,       0,      1, 0),
                                                      (float4)(1,    0,      0,0),
                                                      (float4)(0.5,    0.866,  0, 0),
                                                      (float4)(-0.5,    0.866, 0,0),
                                                      (float4)(-1,    0,     0,0),
                                                      (float4)(-0.5,   -0.866, 0,0),
                                                      (float4)(0.5,   -0.866, 0,0),
                                                      (float4)(0,         0,     0,0)}; 
*/                                             
void compute_app_model_weights(float* app_model_weights, float4 viewdir,__constant float4* app_model_view_directions)
{
    //compute the dot product btw ray dir and canonical directions
    //normalize weights to 1 in the end
    float sum_weights = 0.0f;
    for(short i = 0; i < 8; i++) {
        float cos_angle = -dot(viewdir,app_model_view_directions[i]);
        app_model_weights[i] = (cos_angle > 0.01f) ? cos_angle : 0.0f; //if negative, set to 0
        sum_weights += app_model_weights[i];
    }
    
    for(short i = 0; i < 8; i++) 
        app_model_weights[i] /= sum_weights;
}

/*
void compute_app_model_weights_unormalized(float* app_model_weights, float4 viewdir,__constant float4* app_model_view_directions)
{
    //compute the dot product btw ray dir and canonical directions
    for(short i = 0; i < 8; i++) {
        float cos_angle = -dot(viewdir,app_model_view_directions[i]);
        app_model_weights[i] = (cos_angle > 0.01f) ? cos_angle : 0.0f; //if negative, set to 0
    }
    
}
*/

float view_dep_mixture_model(float x,float16 mixture, float* app_model_weights)
{
    float* mixture_array = (float*)(&mixture);
    float PI = 0;
    for(short i= 0; i < 8; i++)
        PI += app_model_weights[i] * gauss_prob_density(x, mixture_array[2*i],mixture_array[2*i+1]);
    
    return PI;
}

void update_view_dep_app(float x, float w, float* view_dep_w, float* mixture, float * nobs)
{
    for(short i = 0; i < 8; i++)
    {
        if(view_dep_w[i] > 0.01f)   //update only if view weight is positive
        {
            float mean = mixture[2*i];
            float sigma = mixture[2*i+1];
            float n = nobs[i];
            float sk = sigma * sigma * n;
            
            
            if( n  == 0.0f)
            {
                n = w * view_dep_w[i];
                sk = 0.0f;
                mean = x;
            }
            else
            {
                n +=  w * view_dep_w[i];
                float prev_mean = mean;
                mean =  prev_mean + (x-prev_mean)*w*view_dep_w[i] / n;
                sk += w*view_dep_w[i]*(x-prev_mean)*( x - mean );
            }
            mixture[2*i] = mean;
            mixture[2*i+1] = sqrt(sk / n);
            nobs[i] = n;
        }
    }
}
