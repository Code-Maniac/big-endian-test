#include <stdio.h>
#include <stdint.h>
#include <memory.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

/// Structure containing the Device Name
typedef struct
{
    uint8_t m_u8NameArray[8];
} DEVICE_NAME;

/// Structure used for setting the Device Name
#pragma pack(push,1)
typedef struct
{
    uint32_t m_i32UniqueNumber : 21;
    uint16_t m_i16MfgCode : 11;
    uint8_t m_i8NMEAFunctionInstance : 8;
    uint8_t m_i8DeviceFunction : 8;
    uint8_t m_i8Reserved : 1;
    uint8_t m_i8DeviceClass : 7;
    uint8_t m_i8DeviceClassInstance : 4;
    uint8_t m_i8IndustryGroup : 3;
    uint8_t m_i8SelfConfigurable : 1;
} DEVICE_NAME_FIELDS;
#pragma pack(pop)
//------------------------------------------------------------------------------

typedef union
{
    DEVICE_NAME_FIELDS m_NameFields;
    DEVICE_NAME        m_Name;
} DEVNAME;
//------------------------------------------------------------------------------

void CT2KLibPackDeviceNameFields(const DEVICE_NAME_FIELDS* fields, uint8_t* packedFields)
{
    packedFields[0] = (uint8_t)(fields->m_i32UniqueNumber & 0xFF);
    packedFields[1] = (uint8_t)((fields->m_i32UniqueNumber >> 8) & 0xFF);
    packedFields[2] = (uint8_t)((fields->m_i32UniqueNumber >> 16) & 0x1F) | (uint8_t)((fields->m_i16MfgCode << 5) & 0xE0);
    packedFields[3] = (uint8_t)(fields->m_i16MfgCode >> 3);
    packedFields[4] = (uint8_t)fields->m_i8NMEAFunctionInstance;
    packedFields[5] = (uint8_t)fields->m_i8DeviceFunction;
    packedFields[6] = (uint8_t)((fields->m_i8DeviceClass << 1) & 0xFE) | (fields->m_i8Reserved & 0x1);
    packedFields[7] = (uint8_t)((fields->m_i8DeviceClassInstance) & 0x0F) | (uint8_t)((fields->m_i8IndustryGroup << 4) & 0x70) | (uint8_t)((fields->m_i8SelfConfigurable << 7) & 0x80);
}

void CT2KLibUnpackDeviceNameFields(const uint8_t* packedFields, DEVICE_NAME_FIELDS* fields)
{
    fields->m_i32UniqueNumber = (uint32_t)packedFields[0] | ((uint32_t)(packedFields[1] << 8)) | (((uint32_t)packedFields[2] & 0x1F) << 16);
    fields->m_i16MfgCode = (uint16_t)((packedFields[2] & 0xE0) >> 5) | ((uint16_t)(packedFields[3]) << 3);
    fields->m_i8NMEAFunctionInstance = packedFields[4];
    fields->m_i8DeviceFunction = (uint8_t)packedFields[5];
    fields->m_i8Reserved = (uint8_t)packedFields[6] & 0x1;
    fields->m_i8DeviceClass = (uint8_t)((packedFields[6] >> 1) & 0x7F);
    fields->m_i8DeviceClassInstance = (uint8_t)(packedFields[7] & 0x0F);
    fields->m_i8IndustryGroup = (uint8_t)((packedFields[7] >> 4) & 0x07);
    fields->m_i8SelfConfigurable = (uint8_t)((packedFields[7] >> 7) & 0x01);
}

static void print8Bytes(const uint8_t* data)
{
    for(uint8_t i = 0; i < 7; ++i)
    {
        printf("0x%02X ", data[i]);
    }
    printf("0x%02X\n", data[7]);
}

static bool compareBytes(
    const uint8_t* data1,
    const uint8_t* data2,
    const size_t dataSize)
{
    bool equal = true;
    for(size_t i = 0; i < dataSize; ++i)
    {
        if(data1[i] != data2[i])
        {
            equal = false;
            break;
        }
    }
    return equal;
}

int32_t main(int32_t argc, char* argv[])
{
    srand(time(NULL));

    /* for(uint32_t i = 0; i < 50; ++i) */
    {
        DEVNAME dev;
        dev.m_NameFields.m_i32UniqueNumber = 123456;
        dev.m_NameFields.m_i16MfgCode = 466;
        dev.m_NameFields.m_i8NMEAFunctionInstance= 2;
        dev.m_NameFields.m_i8DeviceFunction= 5;
        dev.m_NameFields.m_i8Reserved = 1;
        dev.m_NameFields.m_i8DeviceClass= 34;
        dev.m_NameFields.m_i8DeviceClassInstance = 11;
        dev.m_NameFields.m_i8IndustryGroup = 4;
        dev.m_NameFields.m_i8SelfConfigurable= 1;

        printf("Bit fields:\t");
        print8Bytes(&dev.m_Name.m_u8NameArray[0]);

        DEVICE_NAME devName;
        CT2KLibPackDeviceNameFields(&dev.m_NameFields, &devName.m_u8NameArray[0]);
        printf("Packed:\t\t");
        print8Bytes(&devName.m_u8NameArray[0]);

        CT2KLibUnpackDeviceNameFields(&devName.m_u8NameArray[0], &dev.m_NameFields);
        printf("Unpacked\t");
        print8Bytes(&dev.m_Name.m_u8NameArray[0]);

        printf("Uniq: %u\n", dev.m_NameFields.m_i32UniqueNumber);
        printf("Mfg: %u\n", dev.m_NameFields.m_i16MfgCode);

        printf(compareBytes(&dev.m_Name.m_u8NameArray[0], &devName.m_u8NameArray[0], 8) ? "Equal\n" : "Not Equal\n");
    }

    return 0;
}
