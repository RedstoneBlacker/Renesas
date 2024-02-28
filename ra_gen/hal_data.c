/* generated HAL source file - do not edit */
#include "hal_data.h"
/* I2C Communication Device */
#define RA_NOT_DEFINED (1)
rm_comms_i2c_instance_ctrl_t g_comms_i2c_device1_ctrl;

/* Lower level driver configuration */
const i2c_master_cfg_t g_comms_i2c_device1_lower_level_cfg =
{ .slave = 0x54, .addr_mode = I2C_MASTER_ADDR_MODE_7BIT, };

const rm_comms_cfg_t g_comms_i2c_device1_cfg =
{ .semaphore_timeout = 0xFFFFFFFF, .p_lower_level_cfg = (void*) &g_comms_i2c_device1_lower_level_cfg, .p_extend =
          (void*) &g_comms_i2c_bus0_extended_cfg,
  .p_callback = rm_hs400x_comms_i2c_callback,
#if defined(g_hs400x_sensor0_ctrl)
    .p_context          = g_hs400x_sensor0_ctrl,
#else
  .p_context = (void*) &g_hs400x_sensor0_ctrl,
#endif
        };

const rm_comms_instance_t g_comms_i2c_device1 =
{ .p_ctrl = &g_comms_i2c_device1_ctrl, .p_cfg = &g_comms_i2c_device1_cfg, .p_api = &g_comms_on_comms_i2c, };
rm_hs400x_instance_ctrl_t g_hs400x_sensor0_ctrl;
const rm_hs400x_cfg_t g_hs400x_sensor0_cfg =
{ .p_comms_instance = &g_comms_i2c_device1,
  .temperature_resolution = RM_HS400X_TEMPERATURE_RESOLUTION_14BIT,
  .humidity_resolution = RM_HS400X_HUMIDITY_RESOLUTION_14BIT,
#if 3 == RM_HS400X_CFG_MEASUREMENT_TYPE // Periodic Measurement
    .frequency = RM_HS400X_PERIODIC_MEASUREMENT_FREQUENCY_1HZ,
#endif
  .p_comms_callback = hs400x_comms_i2c_callback,
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = &NULL,
#endif
        };

const rm_hs400x_instance_t g_hs400x_sensor0 =
{ .p_ctrl = &g_hs400x_sensor0_ctrl, .p_cfg = &g_hs400x_sensor0_cfg, .p_api = &g_hs400x_on_hs400x, };
/* I2C Communication Device */
#define RA_NOT_DEFINED (1)
rm_comms_i2c_instance_ctrl_t g_comms_i2c_device0_ctrl;

/* Lower level driver configuration */
const i2c_master_cfg_t g_comms_i2c_device0_lower_level_cfg =
{ .slave = 0x32, .addr_mode = I2C_MASTER_ADDR_MODE_7BIT, };

const rm_comms_cfg_t g_comms_i2c_device0_cfg =
{ .semaphore_timeout = 0xFFFFFFFF, .p_lower_level_cfg = (void*) &g_comms_i2c_device0_lower_level_cfg, .p_extend =
          (void*) &g_comms_i2c_bus0_extended_cfg,
  .p_callback = rm_zmod4xxx_comms_i2c_callback,
#if defined(g_zmod4xxx_sensor0_ctrl)
    .p_context          = g_zmod4xxx_sensor0_ctrl,
#else
  .p_context = (void*) &g_zmod4xxx_sensor0_ctrl,
#endif
        };

const rm_comms_instance_t g_comms_i2c_device0 =
{ .p_ctrl = &g_comms_i2c_device0_ctrl, .p_cfg = &g_comms_i2c_device0_cfg, .p_api = &g_comms_on_comms_i2c, };
zmod4xxx_dev_t g_zmod4xxx_sensor0_dev;
iaq_2nd_gen_handle_t g_zmod4xxx_sensor0_lib_handle;
iaq_2nd_gen_results_t g_zmod4xxx_sensor0_lib_results;
uint8_t g_zmod4xxx_sensor0_product_data[7];
extern rm_zmod4xxx_api_t const g_zmod4xxx_on_zmod4410_iaq_2nd_gen;
extern zmod4xxx_conf g_zmod4410_iaq_2nd_gen_sensor_type[];
rm_zmod4xxx_lib_extended_cfg_t g_zmod4xxx_sensor0_extended_cfg =
{ .lib_type = RM_ZMOD4410_LIB_TYPE_IAQ_2ND_GEN,
  .product_id = 0x2310,
  .p_api = (void*) &g_zmod4xxx_on_zmod4410_iaq_2nd_gen,
  .p_data_set = (void*) g_zmod4410_iaq_2nd_gen_sensor_type,
  .p_product_data = g_zmod4xxx_sensor0_product_data,
  .p_device = (void*) &g_zmod4xxx_sensor0_dev,
  .p_handle = (void*) &g_zmod4xxx_sensor0_lib_handle,
  .p_results = (void*) &g_zmod4xxx_sensor0_lib_results, };
rm_zmod4xxx_instance_ctrl_t g_zmod4xxx_sensor0_ctrl;
const rm_zmod4xxx_cfg_t g_zmod4xxx_sensor0_cfg =
{ .p_comms_instance = &g_comms_i2c_device0,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
  .p_irq_instance = NULL,
  .p_irq_callback = NULL,
#else
    .p_irq_instance = &RA_NOT_DEFINED,
    .p_irq_callback = zmod4xxx_irq_callback,
#endif
#undef RA_NOT_DEFINED
  .p_comms_callback = zmod4xxx_comms_i2c_callback,
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = &NULL,
#endif
  .p_extend = (void*) &g_zmod4xxx_sensor0_extended_cfg, };

const rm_zmod4xxx_instance_t g_zmod4xxx_sensor0 =
{ .p_ctrl = &g_zmod4xxx_sensor0_ctrl, .p_cfg = &g_zmod4xxx_sensor0_cfg, .p_api = &g_zmod4xxx_on_zmod4xxx, };
void g_hal_init(void)
{
    g_common_init ();
}
