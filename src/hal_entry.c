#include "hal_data.h"

FSP_CPP_HEADER
void R_BSP_WarmStart(bsp_warm_start_event_t event);
FSP_CPP_FOOTER



/* TODO: Enable if you want to open I2C bus */
void g_comms_i2c_bus0_quick_setup(void);

/* Quick setup for g_comms_i2c_bus0. */
void g_comms_i2c_bus0_quick_setup(void)
{
    fsp_err_t err;
    i2c_master_instance_t * p_driver_instance = (i2c_master_instance_t *) g_comms_i2c_bus0_extended_cfg.p_driver_instance;

    /* Open I2C driver, this must be done before calling any COMMS API */
    err = p_driver_instance->p_api->open(p_driver_instance->p_ctrl, p_driver_instance->p_cfg);
    assert(FSP_SUCCESS == err);

#if BSP_CFG_RTOS
    /* Create a semaphore for blocking if a semaphore is not NULL */
    if (NULL != g_comms_i2c_bus0_extended_cfg.p_blocking_semaphore)
    {
#if BSP_CFG_RTOS == 1 // AzureOS
        tx_semaphore_create(g_comms_i2c_bus0_extended_cfg.p_blocking_semaphore->p_semaphore_handle,
                            g_comms_i2c_bus0_extended_cfg.p_blocking_semaphore->p_semaphore_name,
                            (ULONG) 0);
#elif BSP_CFG_RTOS == 2 // FreeRTOS
        *(g_comms_i2c_bus0_extended_cfg.p_blocking_semaphore->p_semaphore_handle)
            = xSemaphoreCreateCountingStatic((UBaseType_t) 1, (UBaseType_t) 0, g_comms_i2c_bus0_extended_cfg.p_blocking_semaphore->p_semaphore_memory);
#endif
    }

    /* Create a recursive mutex for bus lock if a recursive mutex is not NULL */
    if (NULL != g_comms_i2c_bus0_extended_cfg.p_bus_recursive_mutex)
    {
#if BSP_CFG_RTOS == 1 // AzureOS
        tx_mutex_create(g_comms_i2c_bus0_extended_cfg.p_bus_recursive_mutex->p_mutex_handle,
                        g_comms_i2c_bus0_extended_cfg.p_bus_recursive_mutex->p_mutex_name,
                        TX_INHERIT);
#elif BSP_CFG_RTOS == 2 // FreeRTOS
        *(g_comms_i2c_bus0_extended_cfg.p_bus_recursive_mutex->p_mutex_handle)
            = xSemaphoreCreateRecursiveMutexStatic(g_comms_i2c_bus0_extended_cfg.p_bus_recursive_mutex->p_mutex_memory);
#endif
    }
#endif
}


/* TODO: Enable if you want to open HS400X */
#define G_HS400X_SENSOR0_NON_BLOCKING (1)

#if G_HS400X_SENSOR0_NON_BLOCKING
volatile bool g_hs400x_completed = false;
volatile rm_hs400x_event_t g_hs400x_event = RM_HS400X_EVENT_ERROR;
#endif

/* TODO: Enable if you want to use a callback */
#define G_HS400X_SENSOR0_CALLBACK_ENABLE (1)
#if G_HS400X_SENSOR0_CALLBACK_ENABLE
void hs400x_comms_i2c_callback(rm_hs400x_callback_args_t * p_args)
{
#if G_HS400X_SENSOR0_NON_BLOCKING
    if (RM_HS400X_EVENT_ERROR != p_args->event)
    {
        g_hs400x_completed = true;
        g_hs400x_event = p_args->event;
    }
#else
    FSP_PARAMETER_NOT_USED(p_args);
#endif
}
#endif

/* Delay */
#define G_HS400X_SENSOR0_DELAY_500  (500)
#define G_HS400X_SENSOR0_DELAY_1000 (1000)
#define G_HS400X_SENSOR0_DELAY_2500 (2500)
#define G_HS400X_SENSOR0_DELAY_MS   (1000)

/* Quick setup for g_hs400x_sensor0.
 * - g_comms_i2c_bus0 must be setup before calling this function
 *     (See Developer Assistance -> g_hs400x_sensor0 -> g_comms_i2c_device1 -> g_comms_i2c_bus0 -> Quick Setup).
 */
void g_hs400x_sensor0_quick_setup(void);

/* Quick setup for g_hs400x_sensor0. */
void g_hs400x_sensor0_quick_setup(void)
{
    fsp_err_t err;

    /* Open HS400X sensor instance, this must be done before calling any HS400X API */
    err = g_hs400x_sensor0.p_api->open(g_hs400x_sensor0.p_ctrl, g_hs400x_sensor0.p_cfg);
    assert(FSP_SUCCESS == err);

#if 3 == RM_HS400X_CFG_MEASUREMENT_TYPE // Periodic Measurement
    /* Start a periodic measurement */
    err = g_hs400x_sensor0.p_api->measurementStart(g_hs400x_sensor0.p_ctrl);
    assert(FSP_SUCCESS == err);
#if G_HS400X_SENSOR0_NON_BLOCKING
    while (!g_hs400x_completed)
    {
        ;
    }
    g_hs400x_completed = false;
#endif
#endif
}


/* Quick getting humidity and temperature values for g_hs400x_sensor0.
 * - g_hs400x_sensor0 must be setup before calling this function.
 */
void g_hs400x_sensor0_quick_getting_humidity_and_temperature(rm_hs400x_data_t * p_data);

/* Quick getting humidity and temperature for g_hs400x_sensor0. */
void g_hs400x_sensor0_quick_getting_humidity_and_temperature(rm_hs400x_data_t * p_data)
{
    fsp_err_t            err;
    rm_hs400x_raw_data_t hs400x_raw_data;
    bool is_valid_data = false;
#if G_HS400X_SENSOR0_NON_BLOCKING
    bool is_complete = false;
#endif
    uint32_t             delay_time;

#if 2 == RM_HS400X_CFG_MEASUREMENT_TYPE // No-Hold Measurement
    /* Start a measurement */
    err = g_hs400x_sensor0.p_api->measurementStart(g_hs400x_sensor0.p_ctrl);
    assert(FSP_SUCCESS == err);
#if G_HS400X_SENSOR0_NON_BLOCKING
    while (!g_hs400x_completed)
    {
        ;
    }
    g_hs400x_completed = false;
#endif
#endif

#if 3 == RM_HS400X_CFG_MEASUREMENT_TYPE // Periodic Measurement
    /* Wait for frequency for Periodic measurement. See table 9 on the page 16 of the datasheet. */
    switch (g_hs400x_sensor0_cfg.frequency)
    {
        case RM_HS400X_PERIODIC_MEASUREMENT_FREQUENCY_2HZ :
            delay_time = G_HS400X_SENSOR0_DELAY_500;
            break;
        case RM_HS400X_PERIODIC_MEASUREMENT_FREQUENCY_1HZ :
            delay_time = G_HS400X_SENSOR0_DELAY_1000;
            break;
        case RM_HS400X_PERIODIC_MEASUREMENT_FREQUENCY_0P4HZ :
            delay_time = G_HS400X_SENSOR0_DELAY_2500;
            break;
        default :
            assert(false);
            break;
    }
#if BSP_CFG_RTOS == 1 // AzureOS
    tx_thread_sleep(delay_time * TX_TIMER_TICKS_PER_SECOND / G_HS400X_SENSOR0_DELAY_MS);
#elif BSP_CFG_RTOS == 2 // FreeRTOS
    vTaskDelay(delay_time * configTICK_RATE_HZ / G_HS400X_SENSOR0_DELAY_MS);
#else // Bare Metal
    R_BSP_SoftwareDelay(delay_time, BSP_DELAY_UNITS_MILLISECONDS);
#endif
#endif

    do
    {
#if G_HS400X_SENSOR0_NON_BLOCKING
        do
        {
#endif
            /* In Hold measurement, start a measurement and read ADC data from HS400X sensor */
            /* In other measurements, read ADC data from HS400X sensor */
            err = g_hs400x_sensor0.p_api->read(g_hs400x_sensor0.p_ctrl, &hs400x_raw_data);
            assert(FSP_SUCCESS == err);
#if G_HS400X_SENSOR0_NON_BLOCKING
            while (!g_hs400x_completed)
            {
                ;
            }
            g_hs400x_completed = false;
#if 2 == RM_HS400X_CFG_MEASUREMENT_TYPE // No-Hold Measurement
            if (RM_HS400X_EVENT_MEASUREMENT_NOT_COMPLETE == g_hs400x_event)
            {
                /* RM_HS400X_EVENT_MEASUREMENT_NOT_COMPLETE is received */
                is_complete = false;
            }
            else
            {
                /* RM_HS400X_EVENT_SUCCESS is received */
                is_complete = true;
            }
#else
            is_complete = true;
#endif
        }
        while (false == is_complete);
#endif

        /* Calculate humidity and temperature values from ADC data */
        err = g_hs400x_sensor0.p_api->dataCalculate(g_hs400x_sensor0.p_ctrl, &hs400x_raw_data, p_data);
        if (FSP_SUCCESS == err)
        {
            is_valid_data = true;
        }
        else if (FSP_ERR_SENSOR_INVALID_DATA == err)
        {
            is_valid_data = false;
        }
        else
        {
            assert(false);
        }
    }
    while (false == is_valid_data);
}


/* TODO: Enable if you want to open ZMOD4XXX */
#define G_ZMOD4XXX_SENSOR0_NON_BLOCKING (1)
#define G_ZMOD4XXX_SENSOR0_IRQ_ENABLE   (0)

#if G_ZMOD4XXX_SENSOR0_NON_BLOCKING
volatile bool g_zmod4xxx_i2c_completed = false;
volatile rm_zmod4xxx_event_t g_zmod4xxx_i2c_callback_event;
#endif
#if G_ZMOD4XXX_SENSOR0_IRQ_ENABLE
volatile bool g_zmod4xxx_irq_completed = false;
#endif

/* TODO: Enable if you want to use a I2C callback */
#define G_ZMOD4XXX_SENSOR0_I2C_CALLBACK_ENABLE (1)
#if G_ZMOD4XXX_SENSOR0_I2C_CALLBACK_ENABLE
void zmod4xxx_comms_i2c_callback(rm_zmod4xxx_callback_args_t * p_args)
{
#if G_ZMOD4XXX_SENSOR0_NON_BLOCKING
    g_zmod4xxx_i2c_callback_event = p_args->event;

    if (RM_ZMOD4XXX_EVENT_ERROR != p_args->event)
    {
        g_zmod4xxx_i2c_completed = true;
    }
#else
    FSP_PARAMETER_NOT_USED(p_args);
#endif
}
#endif

/* TODO: Enable if you want to use a IRQ callback */
#define G_ZMOD4XXX_SENSOR0_IRQ_CALLBACK_ENABLE (0)
#if G_ZMOD4XXX_SENSOR0_IRQ_CALLBACK_ENABLE
void zmod4xxx_irq_callback(rm_zmod4xxx_callback_args_t * p_args)
{
#if G_ZMOD4XXX_SENSOR0_IRQ_ENABLE
    if (RM_ZMOD4XXX_EVENT_MEASUREMENT_COMPLETE == p_args->event)
    {
        g_zmod4xxx_irq_completed = true;
    }
#else
    FSP_PARAMETER_NOT_USED(p_args);
#endif
}
#endif

/* Delay */
#define G_ZMOD4XXX_SENSOR0_DELAY_50     (50)
#define G_ZMOD4XXX_SENSOR0_DELAY_5475   (5475)
#define G_ZMOD4XXX_SENSOR0_DELAY_1990   (1990)
#define G_ZMOD4XXX_SENSOR0_DELAY_1010   (1010)
#define G_ZMOD4XXX_SENSOR0_DELAY_90000  (90000)
#define G_ZMOD4XXX_SENSOR0_DELAY_1500   (1500)
#define G_ZMOD4XXX_SENSOR0_DELAY_2000   (2000)
#define G_ZMOD4XXX_SENSOR0_DELAY_3000   (3000)
#define G_ZMOD4XXX_SENSOR0_DELAY_5000   (5000)
#define G_ZMOD4XXX_SENSOR0_DELAY_MS     (1000)

/* Quick setup for g_zmod4xxx_sensor0.
 * - g_comms_i2c_bus0 must be setup before calling this function
 *     (See Developer Assistance -> g_zmod4xxx_sensor0 -> ZMOD4xxx ***** on rm_zmod4xxx -> g_comms_i2c_device0 -> g_comms_i2c_bus0 -> Quick Setup).
 */
void g_zmod4xxx_sensor0_quick_setup(void);

/* Quick setup for g_zmod4xxx_sensor0. */
void g_zmod4xxx_sensor0_quick_setup(void)
{
    fsp_err_t err;

    /* Open ZMOD4XXX sensor instance, this must be done before calling any ZMOD4XXX API */
    err = g_zmod4xxx_sensor0.p_api->open(g_zmod4xxx_sensor0.p_ctrl, g_zmod4xxx_sensor0.p_cfg);
    assert(FSP_SUCCESS == err);
}


/* Quick getting IAQ 2nd Gen. values for g_zmod4xxx_sensor0.
 * - g_zmod4xxx_sensor0 must be setup before calling this function.
 */
bool g_zmod4xxx_sensor0_quick_getting_iaq_2nd_gen_data(rm_zmod4xxx_iaq_2nd_data_t * p_gas_data, float temperature, float humidity);

/* Quick getting gas data for g_zmod4xxx_sensor0. */
bool g_zmod4xxx_sensor0_quick_getting_iaq_2nd_gen_data(rm_zmod4xxx_iaq_2nd_data_t * p_gas_data, float temperature, float humidity)
{
    fsp_err_t            err;
    rm_zmod4xxx_raw_data_t zmod4xxx_raw_data;
    bool stabilization_complete = false;

    /* Clear callback flags */
#if G_ZMOD4XXX_SENSOR0_IRQ_ENABLE
    g_zmod4xxx_irq_completed = false;
#endif
#if G_ZMOD4XXX_SENSOR0_NON_BLOCKING
    g_zmod4xxx_i2c_completed = false;
#endif

    /* Start the measurement */
    err = g_zmod4xxx_sensor0.p_api->measurementStart(g_zmod4xxx_sensor0.p_ctrl);
    assert(FSP_SUCCESS == err);
#if G_ZMOD4XXX_SENSOR0_NON_BLOCKING
    while (!g_zmod4xxx_i2c_completed)
    {
        ;
    }
    g_zmod4xxx_i2c_completed = false;
#endif

    /* Delay required time. See Table 3 in the ZMOD4410 Programming Manual. */
#if BSP_CFG_RTOS == 1 // AzureOS
    tx_thread_sleep(G_ZMOD4XXX_SENSOR0_DELAY_3000 * TX_TIMER_TICKS_PER_SECOND / G_ZMOD4XXX_SENSOR0_DELAY_MS);
#elif BSP_CFG_RTOS == 2 // FreeRTOS
    vTaskDelay(G_ZMOD4XXX_SENSOR0_DELAY_3000 * configTICK_RATE_HZ / G_ZMOD4XXX_SENSOR0_DELAY_MS);
#else // Bare Metal
    R_BSP_SoftwareDelay(G_ZMOD4XXX_SENSOR0_DELAY_3000, BSP_DELAY_UNITS_MILLISECONDS);
#endif

    do
    {
        /* Wait for the measurement to complete */
#if G_ZMOD4XXX_SENSOR0_IRQ_ENABLE
        while (!g_zmod4xxx_irq_completed)
        {
            ;
        }
        g_zmod4xxx_irq_completed = false;
#else
        err = g_zmod4xxx_sensor0.p_api->statusCheck(g_zmod4xxx_sensor0.p_ctrl);
        assert(FSP_SUCCESS == err);
#if G_ZMOD4XXX_SENSOR0_NON_BLOCKING
        while (!g_zmod4xxx_i2c_completed)
        {
            ;
        }
        g_zmod4xxx_i2c_completed = false;
#endif
#endif

        /* Check if a device error occurs */
        err = g_zmod4xxx_sensor0.p_api->deviceErrorCheck(g_zmod4xxx_sensor0.p_ctrl);
        assert(FSP_SUCCESS == err);
#if G_ZMOD4XXX_SENSOR0_NON_BLOCKING
        while (!g_zmod4xxx_i2c_completed)
        {
            ;
        }
        g_zmod4xxx_i2c_completed = false;

        if ((RM_ZMOD4XXX_EVENT_DEV_ERR_POWER_ON_RESET == g_zmod4xxx_i2c_callback_event) ||
            (RM_ZMOD4XXX_EVENT_DEV_ERR_ACCESS_CONFLICT == g_zmod4xxx_i2c_callback_event))
        {
            /* Error during read of sensor status. Please reset device. */
            while (1)
            {
                ;
            }
        }
#endif

        /* Read ADC data from ZMOD4xxx sensor */
        err = g_zmod4xxx_sensor0.p_api->read(g_zmod4xxx_sensor0.p_ctrl, &zmod4xxx_raw_data);
        if (err == FSP_ERR_SENSOR_MEASUREMENT_NOT_FINISHED)
        {
#if BSP_CFG_RTOS == 1 // AzureOS
            tx_thread_sleep(G_ZMOD4XXX_SENSOR0_DELAY_50 * TX_TIMER_TICKS_PER_SECOND / G_ZMOD4XXX_SENSOR0_DELAY_MS);
#elif BSP_CFG_RTOS == 2 // FreeRTOS
            vTaskDelay(G_ZMOD4XXX_SENSOR0_DELAY_50 * configTICK_RATE_HZ / G_ZMOD4XXX_SENSOR0_DELAY_MS);
#else // Bare Metal
            R_BSP_SoftwareDelay(G_ZMOD4XXX_SENSOR0_DELAY_50, BSP_DELAY_UNITS_MILLISECONDS);
#endif
        }
    }
    while (err == FSP_ERR_SENSOR_MEASUREMENT_NOT_FINISHED);
    assert(FSP_SUCCESS == err);

#if G_ZMOD4XXX_SENSOR0_NON_BLOCKING
    while (!g_zmod4xxx_i2c_completed)
    {
        ;
    }
    g_zmod4xxx_i2c_completed = false;
#endif

        /* Check if a device error occurs */
        err = g_zmod4xxx_sensor0.p_api->deviceErrorCheck(g_zmod4xxx_sensor0.p_ctrl);
        assert(FSP_SUCCESS == err);
#if G_ZMOD4XXX_SENSOR0_NON_BLOCKING
        while (!g_zmod4xxx_i2c_completed)
        {
            ;
        }
        g_zmod4xxx_i2c_completed = false;

        if ((RM_ZMOD4XXX_EVENT_DEV_ERR_POWER_ON_RESET == g_zmod4xxx_i2c_callback_event) ||
            (RM_ZMOD4XXX_EVENT_DEV_ERR_ACCESS_CONFLICT == g_zmod4xxx_i2c_callback_event))
        {
            /* Error during read of sensor status. Please reset device. */
            while (1)
            {
                ;
            }
        }
#endif

    /* Set the current temperature and humidity */
    err = g_zmod4xxx_sensor0.p_api->temperatureAndHumiditySet(g_zmod4xxx_sensor0.p_ctrl, temperature, humidity);
    assert(FSP_SUCCESS == err);

    /* Calculate IAQ 2nd Gen. values from ZMOD4xxx ADC data */
    err = g_zmod4xxx_sensor0.p_api->iaq2ndGenDataCalculate(g_zmod4xxx_sensor0.p_ctrl, &zmod4xxx_raw_data, p_gas_data);
    if (err == FSP_SUCCESS)
    {
        stabilization_complete = true;
    }
    else if(err == FSP_ERR_SENSOR_IN_STABILIZATION)
    {
        stabilization_complete = false;
    }
    else if (err == FSP_ERR_SENSOR_INVALID_DATA)
    {
        stabilization_complete = false;
    }
    else
    {
        assert(false);
    }

    return stabilization_complete;
}

rm_zmod4xxx_iaq_2nd_data_t zmod4410_data ={0};
float temp = 20.0F;
float hum = 51.0F;
rm_hs400x_data_t hs4001_data = {0};

/*******************************************************************************************************************//**
 * main() is generated by the RA Configuration editor and is used to generate threads if an RTOS is used.  This function
 * is called by main() when no RTOS is used.
 **********************************************************************************************************************/
void hal_entry(void)
{
    /* TODO: add your own code here */


    /*Open and initialize the I2C bus*/
    g_comms_i2c_bus0_quick_setup();


    /*Initialize ZMOD4410*/
    g_zmod4xxx_sensor0_quick_setup();


    /*Initialize HS4001 sensor*/
    g_hs400x_sensor0_quick_setup();

    while (1)
        {

            g_zmod4xxx_sensor0_quick_getting_iaq_2nd_gen_data(&zmod4410_data, temp, hum);
            g_hs400x_sensor0_quick_getting_humidity_and_temperature(&hs4001_data);

            R_BSP_SoftwareDelay(1000,BSP_DELAY_UNITS_MILLISECONDS);
        }


#if BSP_TZ_SECURE_BUILD
    /* Enter non-secure code */
    R_BSP_NonSecureEnter();
#endif
}

/*******************************************************************************************************************//**
 * This function is called at various points during the startup process.  This implementation uses the event that is
 * called right before main() to set up the pins.
 *
 * @param[in]  event    Where at in the start up process the code is currently at
 **********************************************************************************************************************/
void R_BSP_WarmStart(bsp_warm_start_event_t event)
{
    if (BSP_WARM_START_RESET == event)
    {
#if BSP_FEATURE_FLASH_LP_VERSION != 0

        /* Enable reading from data flash. */
        R_FACI_LP->DFLCTL = 1U;

        /* Would normally have to wait tDSTOP(6us) for data flash recovery. Placing the enable here, before clock and
         * C runtime initialization, should negate the need for a delay since the initialization will typically take more than 6us. */
#endif
    }

    if (BSP_WARM_START_POST_C == event)
    {
        /* C runtime environment and system clocks are setup. */

        /* Configure pins. */
        IOPORT_CFG_OPEN (&IOPORT_CFG_CTRL, &IOPORT_CFG_NAME);
    }
}

#if BSP_TZ_SECURE_BUILD

FSP_CPP_HEADER
BSP_CMSE_NONSECURE_ENTRY void template_nonsecure_callable ();

/* Trustzone Secure Projects require at least one nonsecure callable function in order to build (Remove this if it is not required to build). */
BSP_CMSE_NONSECURE_ENTRY void template_nonsecure_callable ()
{

}
FSP_CPP_FOOTER

#endif
