/*******************************************************************************
* Title                 :   Accel 3 click
* Filename              :   accel3_hal.c
* Author                :   CAL
* Origin Date           :   04/04/2016
* Notes                 :   None
*******************************************************************************/
/*************** MODULE REVISION LOG ******************************************
*
*    Date    Software Version    Initials     Description
*  04/04/2016   XXXXXXXXXXX        CAL      Interface Created.
*
*******************************************************************************/
/**
 * @file accel3_hal.c
 * @brief This module contains the Hardware Access Layer for Accel 3 Click.
 */
/******************************************************************************
* Includes
*******************************************************************************/
#include "accel3_hal.h"

/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/
#define DUMMY_BYTE  0x00
/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static uint8_t hal_mode = 0;
static uint8_t _i2c_hw_address;


#if defined( __MIKROC_PRO_FOR_ARM__ )
#if defined( STM32F030C6 ) || defined( STM32F107VC ) || \
            defined( STM32F407VG )
static unsigned int( *start_i2c_p )( void );
static unsigned int( *write_i2c_p )( unsigned char slave_address,
                                     unsigned char *buffer,
                                     unsigned long count,
                                     unsigned long end_mode );
static void( *read_i2c_p )( unsigned char slave_address,
                            unsigned char *buffer,
                            unsigned long count,
                            unsigned long end_mode );

#elif defined( LM3S1165 ) || defined( TM4C129ENCZAD )
static void( *enable_i2c_p )( void );
static void( *disable_i2c_p )( void );
static void( *set_slave_address_i2c_p )( unsigned char slave_address,
        unsigned char dir );
static void( *write_i2c_p )( unsigned char data_out,
                             unsigned char mode );
static void( *read_i2c_p )( unsigned char *data,
                            unsigned char mode );
#endif

#elif  defined( __MIKROC_PRO_FOR_AVR__ )
static unsigned char( *busy_i2c_p )( void );
static unsigned char( *status_i2c_p )( void );
static unsigned char( *start_i2c_p )( void );
static void( *stop_i2c_p )( void );
static void( *close_i2c_p )( void );
static void( *write_i2c_p )( unsigned char data_out );
static unsigned char( *read_i2c_p )( unsigned char ack );

#elif  defined( __MIKROC_PRO_FOR_PIC__ )
static unsigned char( *is_idle_i2c_p )( void );
static unsigned char( *start_i2c_p )( void );
static void( *stop_i2c_p )( void );
static void( *restart_i2c_p )( void );
static unsigned char( *write_i2c_p )( unsigned char data_out );
static unsigned char( *read_i2c_p )( unsigned char ack );

#elif defined( __MIKROC_PRO_FOR_PIC32__ )
static unsigned int( *is_idle_i2c_p )( void );
static unsigned int( *start_i2c_p )( void );
static void( *stop_i2c_p )( void );
static unsigned int( *restart_i2c_p )( void );
static unsigned int( *write_i2c_p )( unsigned char data_out );
static unsigned char( *read_i2c_p )( unsigned int ack );

#elif defined( __MIKROC_PRO_FOR_DSPIC__ )
static unsigned int( *is_idle_i2c_p )( void );
static unsigned int( *start_i2c_p )( void );
static void( *stop_i2c_p )( void );
static void( *restart_i2c_p )( void );
static unsigned int( *write_i2c_p )( unsigned char data_out );
static unsigned char( *read_i2c_p )( unsigned int ack );

#elif defined( __MIKROC_PRO_FOR_8051__ )
static unsigned char ( *status_i2c_p )( void );
static unsigned char( *start_i2c_p )( void );
static void( *stop_i2c_p )( void );
static void( *close_i2c_p )( void );
static void( *write_i2c_p )( unsigned char data_out );
static unsigned char( *read_i2c_p )( unsigned char ack );

#elif defined( __MIKROC_PRO_FOR_FT90x__ )
static void( *soft_reset_i2c_p )( void );
static void( *set_slave_address_i2c_p )( unsigned char slave_address );
static unsigned char( *write_i2c_p )( unsigned char data_out );
static unsigned char( *read_i2c_p )( unsigned char *data_in );
static unsigned char( *write_bytes_i2c_p )( unsigned char *buffer,
        unsigned int count );
static unsigned char( *read_bytes_i2c_p )( unsigned char *buffer,
        unsigned int count );
static unsigned char( *write_10bit_i2c_p )( unsigned char data_out,
        unsigned int address_10bit );
static unsigned char( *read_10bit_i2c_p )( unsigned char *data_in,
        unsigned int address_10bit );
#endif



#if defined( __MIKROC_PRO_FOR_ARM__ )       || \
        defined( __MIKROC_PRO_FOR_DSPIC__ )
static void         ( *write_spi_p )            ( unsigned int data_out );
static unsigned int ( *read_spi_p )             ( unsigned int buffer );

#elif defined( __MIKROC_PRO_FOR_AVR__ )     || \
          defined( __MIKROC_PRO_FOR_PIC__ )     || \
          defined( __MIKROC_PRO_FOR_8051__ )
static void         ( *write_spi_p )            ( unsigned char data_out );
static unsigned char( *read_spi_p )             ( unsigned char dummy );

#elif defined( __MIKROC_PRO_FOR_PIC32__ )
static void         ( *write_spi_p )            ( unsigned long data_out );
static unsigned long( *read_spi_p )             ( unsigned long buffer );

#elif defined( __MIKROC_PRO_FOR_FT90x__ )
static void         ( *write_spi_p )            ( unsigned char data_out );
static unsigned char( *read_spi_p )             ( unsigned char dummy );
static void         ( *write_bytes_spi_p )      ( unsigned char *data_out,
        unsigned int count );
static void         ( *read_bytes_spi_p )       ( unsigned char *buffer,
        unsigned int count );
#endif

#if defined( __MIKROC_PRO_FOR_ARM__ )     || \
        defined( __MIKROC_PRO_FOR_AVR__ )     || \
        defined( __MIKROC_PRO_FOR_PIC__ )     || \
        defined( __MIKROC_PRO_FOR_PIC32__ )   || \
        defined( __MIKROC_PRO_FOR_DSPIC__ )   || \
        defined( __MIKROC_PRO_FOR_8051__ )    || \
        defined( __MIKROC_PRO_FOR_FT90x__ )
extern sfr sbit ACCEL_3_CS;
extern sfr sbit ACCEL_3_RST;

#elif defined( __GNUC__ )
#define ACCEL_3_CS
#define ACCEL_3_RST
#endif

/******************************************************************************
* Function Prototypes
*******************************************************************************/

static inline void accel_3_hal_cs_high( void );

static inline void accel_3_hal_cs_low( void );

static inline void accel_3_hal_reset( void );

static inline void accel_3_hal_delay( uint32_t ms );
/******************************************************************************
* Function Definitions
*******************************************************************************/
void accel3_hal_init(uint8_t address_id, uint8_t mode )
{
    if ( mode == 0 )
    {
#if defined( __MIKROC_PRO_FOR_ARM__ )
#if defined( STM32F030C6 ) || defined( STM32F107VC ) || \
                defined( STM32F407VG )
        start_i2c_p = I2C_Start_Ptr;
        write_i2c_p = I2C_Write_Ptr;
        read_i2c_p = I2C_Read_Ptr;

#elif defined( LM3S1165 ) || defined( TM4C129ENCZAD )
        enable_i2c_p = I2C_Enable_Ptr;
        disable_i2c_p = I2C_Disable_Ptr;
        set_slave_address_i2c_p = I2C_Master_Slave_Addr_Set_Ptr;
        write_i2c_p = I2C_Write_Ptr;
        read_i2c_p = I2C_Read_Ptr;
#endif

#elif defined( __MIKROC_PRO_FOR_AVR__ )
#if defined( ATMEGA32 )
        busy_i2c_p = TWI_Busy;
        status_i2c_p = TWI_Status;
        close_i2c_p = TWI_Close;
        start_i2c_p = TWI_Start;
        stop_i2c_p = TWI_Stop;
        write_i2c_p = TWI_Write;
        read_i2c_p = TWI_Read;
#elif defined( ATXMEGA32A4 )
        busy_i2c_p = TWIC_Busy;
        status_i2c_p = TWIC_Status;
        close_i2c_p = TWIC_Close;
        start_i2c_p = TWIC_Start;
        stop_i2c_p = TWIC_Stop;
        write_i2c_p = TWIC_Write;
        read_i2c_p = TWIC_Read;
#endif

#elif defined( __MIKROC_PRO_FOR_PIC__ )
        is_idle_i2c_p = I2C1_Is_Idle;
        start_i2c_p = I2C1_Start;
        stop_i2c_p = I2C1_Stop;
        restart_i2c_p = I2C1_Repeated_Start;
        write_i2c_p = I2C1_Wr;
        read_i2c_p = I2C1_Rd;

#elif defined( __MIKROC_PRO_FOR_PIC32__ )
        is_idle_i2c_p = I2C_Is_Idle_Ptr;
        start_i2c_p = I2C_Start_Ptr;
        stop_i2c_p = I2C_Stop_Ptr;
        restart_i2c_p = I2C_Restart_Ptr;
        write_i2c_p = I2C_Write_Ptr;
        read_i2c_p = I2C_Read_Ptr;

#elif defined( __MIKROC_PRO_FOR_DSPIC__ )
        is_idle_i2c_p = I2C2_Is_Idle;
        start_i2c_p = I2C2_Start;
        stop_i2c_p = I2C2_Stop;
        restart_i2c_p = I2C2_Restart;
        write_i2c_p = I2C2_Write;
        read_i2c_p = I2C2_Read;

#elif defined( __MIKROC_PRO_FOR_8051__ )
        status_i2c_p = TWI_Status;
        close_i2c_p = TWI_Close;
        start_i2c_p = TWI_Start;
        stop_i2c_p = TWI_Stop;
        write_i2c_p = TWI_Write;
        read_i2c_p = TWI_Read;

#elif defined( __MIKROC_PRO_FOR_FT90x__ )
        soft_reset_i2c_p = I2CM_Soft_Reset_Ptr;
        set_slave_address_i2c_p = I2CM_Set_Slave_Address_Ptr;
        write_i2c_p = I2CM_Write_Ptr;
        read_i2c_p = I2CM_Read_Ptr;
        write_bytes_i2c_p = I2CM_Write_Bytes_Ptr;
        read_bytes_i2c_p = I2CM_Read_Bytes_Ptr;
        write_10bit_i2c_p = I2CM_Write_10Bit_Ptr;
        read_10bit_i2c_p = I2CM_Read_10Bit_Ptr;
#endif


#if defined( __MIKROC_PRO_FOR_ARM__ )   ||  \
            defined( __MIKROC_PRO_FOR_FT90x__ )
        _i2c_hw_address = address_id;
        accel_3_hal_cs_high();
#else
        _i2c_hw_address = ( address_id << 1 );
        accel_3_hal_cs_high();
#endif
    }
    else if ( mode == 1 )
    {


#if defined( __MIKROC_PRO_FOR_ARM__ )   || \
            defined( __MIKROC_PRO_FOR_AVR__ )   || \
            defined( __MIKROC_PRO_FOR_DSPIC__ ) || \
            defined( __MIKROC_PRO_FOR_PIC32__ ) || \
            defined( __MIKROC_PRO_FOR_8051__ )
        write_spi_p             = SPI_Wr_Ptr;
        read_spi_p              = SPI_Rd_Ptr;

#elif defined( __MIKROC_PRO_FOR_PIC__ )
        write_spi_p             = SPI1_Write;
        read_spi_p              = SPI1_Read;

#elif defined( __MIKROC_PRO_FOR_FT90x__ )
        write_spi_p             = SPIM_Wr_Ptr;
        read_spi_p              = SPIM_Rd_Ptr;
        write_bytes_spi_p       = SPIM_WrBytes_Ptr;
        read_bytes_spi_p        = SPIM_RdBytes_Ptr;
#endif
        accel_3_hal_cs_high();
    }

    hal_mode = mode;

}

void accel3_hal_write(uint8_t *command, uint8_t *buffer,
                      uint8_t count )
{
    if ( hal_mode == 0 )
    {
        uint8_t temp[ MAX_BUFF_SIZE ];
        uint8_t cmd_size = COMMAND_SIZE;
        uint8_t ptr = count;
        uint16_t i = 0;

        while ( cmd_size-- )
            temp[ i++ ] = *( command++ );

        while ( ptr-- )
            temp[ i++ ] = *( buffer++ );

#if defined(__MIKROC_PRO_FOR_ARM__)
#if defined( STM32F030C6 ) || defined( STM32F107VC ) || \
                defined( STM32F407VG )
        start_i2c_p();
        write_i2c_p( _i2c_hw_address, temp, i, END_MODE_STOP );

#elif defined( LM3S1165 ) || defined( TM4C129ENCZAD )
        set_slave_address_i2c_p( _i2c_hw_address, _I2C_DIR_MASTER_TRANSMIT );

        if ( i == 2 )
        {
            write_i2c_p( *( buffer++ ), _I2C_MASTER_MODE_BURST_SEND_START );
            write_i2c_p( *buffer, _I2C_MASTER_MODE_BURST_SEND_STOP );

        } else {

            write_i2c_p( *( buffer++ ), _I2C_MASTER_MODE_BURST_SEND_START );

            while ( i-- > 1 )
                write_i2c_p( *( buffer++ ), _I2C_MASTER_MODE_BURST_SEND_CONT );

            write_i2c_p( *buffer, _I2C_MASTER_MODE_BURST_SEND_FINISH );
        }
#endif

#elif defined(__MIKROC_PRO_FOR_FT90x__)
        set_slave_address_i2c_p( _i2c_hw_address );
        write_bytes_i2c_p( temp, i );

#elif defined(__MIKROC_PRO_FOR_AVR__)   || \
              defined(__MIKROC_PRO_FOR_8051__)  || \
              defined(__MIKROC_PRO_FOR_DSPIC__) || \
              defined(__MIKROC_PRO_FOR_PIC32__) || \
              defined(__MIKROC_PRO_FOR_PIC__)

        start_i2c_p();
        write_i2c_p( _i2c_hw_address + WRITE );

        for ( i = 0; i <= count; i++ )
            write_i2c_p(temp[i]);

        stop_i2c_p();

#endif

    }
    else if ( hal_mode == 1 )
    {

        accel_3_hal_cs_low();
        while ( count-- )
        {
            write_spi_p( *( buffer++ ) );
        }
        accel_3_hal_cs_high();
    }

}

void accel3_hal_read(uint8_t *command, uint8_t *buffer,
                     uint8_t count )
{
    if ( hal_mode == 0 )
    {
        uint8_t cmd_size = COMMAND_SIZE;

#if defined(__MIKROC_PRO_FOR_ARM__)
#if defined( STM32F030C6 ) || defined( STM32F107VC ) || \
                defined( STM32F407VG )
        start_i2c_p();
        write_i2c_p( _i2c_hw_address, command, cmd_size, END_MODE_RESTART );
        read_i2c_p( _i2c_hw_address, buffer, count, END_MODE_STOP );

#elif defined( LM3S1165 ) || defined( TM4C129ENCZAD )
        set_slave_address_i2c_p( _i2c_hw_address, _I2C_DIR_MASTER_TRANSMIT );
        if ( cmd_size == 1 )
        {
            write_i2c_p( *command, _I2C_MASTER_MODE_SINGLE_SEND );

        } else if ( cmd_size == 2 ) {

            write_i2c_p( *( command++ ), _I2C_MASTER_MODE_BURST_SEND_START );
            write_i2c_p( *command, _I2C_MASTER_MODE_BURST_SEND_STOP  );

        } else {

            write_i2c_p( *( command++ ), _I2C_MASTER_MODE_BURST_SEND_START );
            cmd_size--;

            while ( cmd_size-- > 1 )
                write_i2c_p( *( command++ ), _I2C_MASTER_MODE_BURST_SEND_CONT );

            write_i2c_p( *command, _I2C_MASTER_MODE_BURST_SEND_FINISH );
        }

        set_slave_address_i2c_p( _i2c_hw_address, _I2C_DIR_MASTER_RECEIVE );
        if ( count == 1 )
        {
            read_i2c_p( buffer, _I2C_MASTER_MODE_BURST_SINGLE_RECEIVE );

        } else {

            read_i2c_p( buffer++ , _I2C_MASTER_MODE_BURST_RECEIVE_START ) )
            count--;

            while ( count-- > 1 )
            read_i2c_p( buffer++ , _I2C_MASTER_MODE_BURST_SEND_CONT );

            read_i2c_p( buffer, _I2C_MASTER_MODE_BURST_SEND_FINISH );
        }
#endif

#elif defined(__MIKROC_PRO_FOR_FT90x__)
        set_slave_address_i2c_p( _i2c_hw_address );

        if ( cmd_size == 1 )
            write_i2c_p( *command );
        else
            write_bytes_i2c_p( command, cmd_size );

        read_bytes_i2c_p( buffer, count );

#elif defined( __MIKROC_PRO_FOR_AVR__ )    || \
              defined( __MIKROC_PRO_FOR_PIC32__ )  || \
              defined( __MIKROC_PRO_FOR_8051__ )


        start_i2c_p();
        write_i2c_p( _i2c_hw_address | WRITE );
        write_i2c_p( *command );
        start_i2c_p();
        write_i2c_p( _i2c_hw_address | READ );
        if ( count == 1 )
        {
            *buffer = read_i2c_p( 0u );

        } else {

            while ( count-- > 1 )
                *( buffer++ ) = read_i2c_p( 1 );

            *buffer = read_i2c_p( 0u );

        }
        stop_i2c_p();

#elif defined(__MIKROC_PRO_FOR_PIC__)  || \
              defined(__MIKROC_PRO_FOR_DSPIC__)
        start_i2c_p();
        write_i2c_p( _i2c_hw_address + WRITE );

        while ( cmd_size-- )
            write_i2c_p( *( command++ ) );

        restart_i2c_p();
        write_i2c_p( _i2c_hw_address + READ );

        if ( count == 1 )
        {
            *buffer = read_i2c_p( 0 );

        } else {

            while ( count > 1 )
            {
                *( buffer++ ) = read_i2c_p( 1 );
                count--;
            }

            *buffer = read_i2c_p( 0 );
        }
        stop_i2c_p();
#endif
    }
    else if ( hal_mode == 1 )
    {

        accel_3_hal_cs_low();
        while ( count-- )
            *( buffer++ ) = read_spi_p( DUMMY_BYTE );
        accel_3_hal_cs_high();
    }

}

static inline void accel_3_hal_cs_high( void )
{
    ACCEL_3_CS = 1;
}

static inline void accel_3_hal_cs_low( void )
{
    ACCEL_3_CS = 0;
    accel_3_hal_delay( 1 );
}

static inline void accel_3_hal_reset( void )
{
    ACCEL_3_RST = 0;
    accel_3_hal_delay( 10 );
    ACCEL_3_RST = 1;
}

static inline void accel_3_hal_delay( uint32_t ms )
{
    while ( ms--  )
        Delay_1ms( );
}


/*************** END OF FUNCTIONS *********************************************/