/*
 * File: controller_simulink_model.c
 *
 * Code generated for Simulink model 'controller_simulink_model'.
 *
 * Model version                  : 1.11
 * Simulink Coder version         : 8.10 (R2016a) 10-Feb-2016
 * C/C++ source code generated on : Thu May 28 04:03:11 2020
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#include "controller_simulink_model.h"

/* Block signals and states (auto storage) */
DW rtDW;

/* External inputs (root inport signals with auto storage) */
ExtU rtU;

/* External outputs (root outports fed by signals with auto storage) */
ExtY rtY;

/* Real-time model */
RT_MODEL rtM_;
RT_MODEL *const rtM = &rtM_;

/* Model step function */
void controller_simulink_model_step(void)
{
  int32_T dir;
  real_T rtb_count2rad1;
  real_T rtb_DiscreteFIRFilter;
  real_T rtb_TSamp;
  real_T rtb_Sum;
  real_T rtb_NOut;
  real_T rtb_Saturate;
  real_T Diff;

  /* Gain: '<Root>/count2rad1' incorporates:
   *  Constant: '<Root>/ '
   *  DataTypeConversion: '<Root>/Data Type Conversion'
   *  Inport: '<Root>/speedInput'
   *  Sum: '<Root>/Sum1'
   */
  rtb_count2rad1 = ((real_T)rtU.speedInput - 128.0) * 4.45;

  /* SampleTimeMath: '<S2>/TSamp' incorporates:
   *  DataTypeConversion: '<Root>/Data Type Conversion1'
   *  Gain: '<Root>/  '
   *  Inport: '<Root>/QEIcount'
   *
   * About '<S2>/TSamp':
   *  y = u * K where K = 1 / ( w * Ts )
   */
  rtb_TSamp = 0.1745329251994329 * (real_T)rtU.shaftPos_count * 20.0;

  /* Sum: '<S2>/Diff' incorporates:
   *  UnitDelay: '<S2>/UD'
   *
   * Block description for '<S2>/Diff':
   *
   *  Add in CPU
   *
   * Block description for '<S2>/UD':
   *
   *  Store in Global RAM
   */
  Diff = rtb_TSamp - rtDW.UD_DSTATE;

  /* DiscreteFir: '<Root>/Discrete FIR Filter' */
  rtb_DiscreteFIRFilter = Diff * 0.2;
  for (dir = rtDW.DiscreteFIRFilter_circBuf; dir < 4; dir++) {
    rtb_DiscreteFIRFilter += rtDW.DiscreteFIRFilter_states[dir] * 0.2;
  }

  for (dir = 0; dir < rtDW.DiscreteFIRFilter_circBuf; dir++) {
    rtb_DiscreteFIRFilter += rtDW.DiscreteFIRFilter_states[dir] * 0.2;
  }

  /* End of DiscreteFir: '<Root>/Discrete FIR Filter' */

  /* Sum: '<Root>/Sum' */
  rtb_Sum = rtb_count2rad1 - rtb_DiscreteFIRFilter;

  /* Product: '<S3>/NOut' incorporates:
   *  DiscreteIntegrator: '<S3>/Filter'
   *  Inport: '<Root>/kD'
   *  Inport: '<Root>/kN'
   *  Product: '<S3>/DOut'
   *  Sum: '<S3>/SumD'
   */
  rtb_NOut = (rtb_Sum * rtU.kD - rtDW.Filter_DSTATE) * (real_T)rtU.kN;

  /* Sum: '<S3>/Sum' incorporates:
   *  DiscreteIntegrator: '<S3>/Integrator'
   *  Inport: '<Root>/kP'
   *  Product: '<S3>/POut'
   */
  rtb_Saturate = (rtb_Sum * rtU.kP + rtDW.Integrator_DSTATE) + rtb_NOut;

  /* Saturate: '<S3>/Saturate' */
  if (rtb_Saturate > 400.0) {
    rtb_Saturate = 400.0;
  } else {
    if (rtb_Saturate < -400.0) {
      rtb_Saturate = -400.0;
    }
  }

  /* End of Saturate: '<S3>/Saturate' */

  /* Outport: '<Root>/PWM_dutyCycle' incorporates:
   *  Abs: '<Root>/Abs'
   */
  rtY.PWM_dutyCycle = fabs(rtb_Saturate);

  /* MATLAB Function: '<Root>/Direction' */
  /* MATLAB Function 'Direction': '<S1>:1' */
  /* '<S1>:1:2' if (u>=0) */
  if (rtb_Saturate >= 0.0) {
    /* '<S1>:1:3' dir = 1; */
    dir = 1;
  } else {
    /* '<S1>:1:4' else */
    /* '<S1>:1:5' dir = 0; */
    dir = 0;
  }

  /* Outport: '<Root>/direction ' incorporates:
   *  DataTypeConversion: '<Root>/Data Type Conversion3'
   *  MATLAB Function: '<Root>/Direction'
   */
  /* '<S1>:1:7' y = dir; */
  rtY.direction = (dir != 0);

  /* Outport: '<Root>/speedInput_rad//sec' */
  rtY.speedInput_radsec = rtb_count2rad1;

  /* Outport: '<Root>/PWM_dutyCommand' */
  rtY.PWM_dutyCommand = rtb_Saturate;

  /* Outport: '<Root>/speedMeasured_rad//sec' */
  rtY.speedMeasured_radsec = rtb_DiscreteFIRFilter;

  /* Outport: '<Root>/speedError_rad//sec' */
  rtY.speedError_radsec = rtb_Sum;

  /* Update for UnitDelay: '<S2>/UD'
   *
   * Block description for '<S2>/UD':
   *
   *  Store in Global RAM
   */
  rtDW.UD_DSTATE = rtb_TSamp;

  /* Update for DiscreteFir: '<Root>/Discrete FIR Filter' */
  /* Update circular buffer index */
  rtDW.DiscreteFIRFilter_circBuf--;
  if (rtDW.DiscreteFIRFilter_circBuf < 0) {
    rtDW.DiscreteFIRFilter_circBuf = 3;
  }

  /* Update circular buffer */
  rtDW.DiscreteFIRFilter_states[rtDW.DiscreteFIRFilter_circBuf] = Diff;

  /* End of Update for DiscreteFir: '<Root>/Discrete FIR Filter' */

  /* Update for DiscreteIntegrator: '<S3>/Integrator' incorporates:
   *  Inport: '<Root>/kI'
   *  Product: '<S3>/IOut'
   */
  rtDW.Integrator_DSTATE += rtb_Sum * rtU.kI * 0.05;

  /* Update for DiscreteIntegrator: '<S3>/Filter' */
  rtDW.Filter_DSTATE += 0.05 * rtb_NOut;
}

/* Model initialize function */
void controller_simulink_model_initialize(void)
{
  /* (no initialization code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
