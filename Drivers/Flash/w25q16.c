#include "w25q16.h"

// ===== 설정 =====
extern SPI_HandleTypeDef hspi1; //spi1사용 예정

#define W25Q16_CS_PORT   GPIOB
#define W25Q16_CS_PIN    GPIO_PIN_0

// ===== 명령어 =====
#define CMD_READ_ID        0x9F  //Flash가 0xEF4015와 같이 EF(Winbond), 4015(W25Q16) 형태의 제조사+디바이스 정보 반환
#define CMD_READ_DATA      0x03  //W25Q16_Read(0x000000, buffer, 4096); >> 0x000000(3바이트주소)부터 4096바이트 데이터를 읽어서 buffer에 저장, 읽고자 하는 데이터 길이는 MCU가 결정함
#define CMD_PAGE_PROGRAM   0x02  //page단위 쓰기 명령어, page경계 넘으면 꼬임
#define CMD_SECTOR_ERASE   0x20  //명렁어 이후 보내는 3바이트 주소가 포함된 sector전체가 지워짐 즉, sector 0 : 0x000000 ~ 0x000FFF, sector 1 : 0x001000 ~ 0x001FFF라서 하위 12bit는 의미없음
#define CMD_WRITE_ENABLE   0x06  //Flash는 기본 보호상태라 해당 명령어 이후 쓰기 및 삭제 가능
#define CMD_READ_STATUS    0x05  //Flash의 상태 레지스터(Status Register)를 읽는 명령어, bit0 (BUSY) → 1: 작업 중 / 0: 완료

/*읽기(Read)  → 단위 없음 (연속으로 아무 길이든 가능)
쓰기(Write) → Page 단위 (256 bytes 제한)
지우기(Erase) → Sector 단위 (4KB)*/

// ===== 내부 함수 =====

static void CS_LOW(void)
{
    HAL_GPIO_WritePin(W25Q16_CS_PORT, W25Q16_CS_PIN, GPIO_PIN_RESET);
}

static void CS_HIGH(void)
{
    HAL_GPIO_WritePin(W25Q16_CS_PORT, W25Q16_CS_PIN, GPIO_PIN_SET);
}

static void SPI_TX(uint8_t *data, uint16_t len) //data 버퍼에 있는 데이터를 len만큼 전송하는 내부 함수
{
    HAL_SPI_Transmit(&hspi1, data, len, HAL_MAX_DELAY);
}

static void SPI_RX(uint8_t *data, uint16_t len) //data 버퍼에 len만큼 채워 넣는 내부 함수
{
    HAL_SPI_Receive(&hspi1, data, len, HAL_MAX_DELAY);
}

// ===== Low Level =====

static void WriteEnable(void)
{
    uint8_t cmd = CMD_WRITE_ENABLE;

    CS_LOW();
    SPI_TX(&cmd, 1);
    CS_HIGH();
}

uint8_t W25Q16_IsBusy(void) //Flash가 현재 작업 중인지(Busy 상태인지) 확인하는 함수
{
    uint8_t cmd = CMD_READ_STATUS;
    uint8_t status;

    CS_LOW();
    SPI_TX(&cmd, 1);
    SPI_RX(&status, 1);
    CS_HIGH();

    return (status & 0x01); //1 → Busy (아직 작업 중) , 0 → Ready (사용 가능)
}

static void WaitBusy(void)
{
    while(W25Q16_IsBusy());
}

// ===== API =====


uint32_t W25Q16_ReadID(void)
{
    uint8_t cmd = CMD_READ_ID;
    uint8_t id[3]; //Flash로 부터 읽어온 ID 3byte를 저장할 버퍼

    CS_LOW();
    SPI_TX(&cmd, 1);
    SPI_RX(id, 3);
    CS_HIGH();

    return (id[0] << 16) | (id[1] << 8) | id[2];
}

void W25Q16_Read(uint32_t addr, uint8_t *buf, uint16_t len)
{
    uint8_t cmd[4];

    cmd[0] = CMD_READ_DATA;
    cmd[1] = (addr >> 16) & 0xFF;
    cmd[2] = (addr >> 8) & 0xFF;
    cmd[3] = addr & 0xFF;

    CS_LOW();
    SPI_TX(cmd, 4);
    SPI_RX(buf, len);
    CS_HIGH();
}

void W25Q16_EraseSector(uint32_t addr)
{
    uint8_t cmd[4];

    WriteEnable();

    cmd[0] = CMD_SECTOR_ERASE;
    cmd[1] = (addr >> 16) & 0xFF;
    cmd[2] = (addr >> 8) & 0xFF;
    cmd[3] = addr & 0xFF;

    CS_LOW();
    SPI_TX(cmd, 4);
    CS_HIGH();

    WaitBusy();
}

void W25Q16_Write(uint32_t addr, uint8_t *buf, uint16_t len)
{
    while(len > 0)
    {
        uint16_t page_offset = addr % 256;
        uint16_t chunk = 256 - page_offset;

        if(chunk > len)
            chunk = len;

        WriteEnable();

        uint8_t cmd[4];
        cmd[0] = CMD_PAGE_PROGRAM;
        cmd[1] = (addr >> 16) & 0xFF;
        cmd[2] = (addr >> 8) & 0xFF;
        cmd[3] = addr & 0xFF;

        CS_LOW();
        SPI_TX(cmd, 4);
        SPI_TX(buf, chunk);
        CS_HIGH();

        WaitBusy();

        addr += chunk;
        buf  += chunk;
        len  -= chunk;
    }
}
