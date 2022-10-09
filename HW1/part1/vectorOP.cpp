#include "PPintrin.h"

// implementation of absSerial(), but it is vectorized using PP intrinsics
void absVector(float *values, float *output, int N)
{
  __pp_vec_float x;
  __pp_vec_float result;
  __pp_vec_float zero = _pp_vset_float(0.f);
  __pp_mask maskAll, maskIsNegative, maskIsNotNegative;

  //  Note: Take a careful look at this loop indexing.  This example
  //  code is not guaranteed to work when (N % VECTOR_WIDTH) != 0.
  //  Why is that the case?
  for (int i = 0; i < N; i += VECTOR_WIDTH)
  {

    // All ones
    maskAll = _pp_init_ones();

    // All zeros
    maskIsNegative = _pp_init_ones(0);

    // Load vector of values from contiguous memory addresses
    _pp_vload_float(x, values + i, maskAll); // x = values[i];

    // Set mask according to predicate
    _pp_vlt_float(maskIsNegative, x, zero, maskAll); // if (x < 0) {

    // Execute instruction using mask ("if" clause)
    _pp_vsub_float(result, zero, x, maskIsNegative); //   output[i] = -x;

    // Inverse maskIsNegative to generate "else" mask
    maskIsNotNegative = _pp_mask_not(maskIsNegative); // } else {

    // Execute instruction ("else" clause)
    _pp_vload_float(result, values + i, maskIsNotNegative); //   output[i] = x; }

    // Write results back to memory
    _pp_vstore_float(output + i, result, maskAll);
  }
}

void clampedExpVector(float *values, int *exponents, float *output, int N)
{

  __pp_vec_float values_x, output_x, max; 
  __pp_vec_int exp_x, exp_tmp, ones;
  __pp_mask pp_mask, cond_mask;

  for (int i = 0; i < N; i += VECTOR_WIDTH)
  {
    // check boundary condition
    pp_mask = i + VECTOR_WIDTH > N ? _pp_init_ones(N % VECTOR_WIDTH) : _pp_init_ones(VECTOR_WIDTH);
    
    // load vector for pp operations
    _pp_vload_float(values_x, values + i, pp_mask);
    _pp_vload_float(output_x, output + i, pp_mask);
    _pp_vload_int(exp_x, exponents + i, pp_mask);
    
    // constant
    _pp_vset_int(ones, 1, pp_mask); 
    _pp_vset_float(max, 9.999999f, pp_mask); 

    // reset cond mask and exp value
    _pp_vset_int(exp_tmp, 1, pp_mask); // TODO: why we cannot access _pp_vset
    cond_mask = _pp_init_ones(0);

    // initialization for each iteration    
    _pp_vmove_float(output_x, values_x, pp_mask); // init values^1 to output
    _pp_vlt_int(cond_mask, exp_x, exp_tmp, pp_mask); // check whether exp=0
    _pp_vset_float(output_x, 1.f, cond_mask); // assign 1.f to those who exp=0
    
    // mask was set if further multilication is needed
    cond_mask = _pp_mask_not(cond_mask);
    cond_mask = _pp_mask_and(cond_mask, pp_mask);

    while (_pp_cntbits(cond_mask) > 0)
    {
      // update cond_mask
      _pp_vlt_int(cond_mask, exp_tmp, exp_x, pp_mask); // update first if any element of exp_x == 1

      _pp_vmult_float(output_x, output_x, values_x, cond_mask);
      _pp_vadd_int(exp_tmp, exp_tmp, ones, cond_mask);

    }

    // assign 9.999999f to those who > 9.999999f 
    _pp_vlt_float(cond_mask, max, output_x, pp_mask);
    _pp_vset_float(output_x, 9.999999f, cond_mask);

    // assign back to the output array 
    _pp_vstore_float(output + i, output_x, pp_mask);
  }
}

// returns the sum of all elements in values
// You can assume N is a multiple of VECTOR_WIDTH
// You can assume VECTOR_WIDTH is a power of 2
float arraySumVector(float *values, int N)
{

  //
  // PP STUDENTS TODO: Implement your vectorized version of arraySumSerial here
  //
  __pp_mask all = _pp_init_ones(VECTOR_WIDTH);
  __pp_vec_float sum_x, sum_x_copy, values_x;
  sum_x = _pp_vset_float(0.0f);

  // O(N / VECTOR_WIDTH )
  for (int i = 0; i < N; i += VECTOR_WIDTH)
  {
    _pp_vload_float(values_x, values + i, all);
    _pp_vadd_float(sum_x, sum_x, values_x, all);
  }

  unsigned vector_w = VECTOR_WIDTH;
  // O(log(VECTOR_WIDTH))
  while (vector_w > 1) {
    _pp_hadd_float(sum_x, sum_x);
    _pp_interleave_float(sum_x_copy, sum_x);
    _pp_vmove_float(sum_x, sum_x_copy, all);
    vector_w /= 2;
  }

  return sum_x.value[0];
}