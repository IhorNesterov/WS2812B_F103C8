#include "NOS_UART.h"

NOS_UART_Struct* NOS_UART_ReceiveReset(NOS_UART_Struct* data)
{
    data->lastMessageSize = data->currMessageLenght;
    data->rx_buff_ptr = data->rx_buff;
    data->currMessageLenght = 0;
    data->startReceive = false;
    data->rx_flag = true;
    return data;
}

NOS_UART_Struct* NOS_UART_ContinueReceive(NOS_UART_Struct* data)
{
    data->rx_buff[data->currMessageLenght] = *data->rx_buff_ptr;
    ++data->rx_buff_ptr;
    return data;
}

void NOS_UART_ReceiveAbort(NOS_UART_Struct* data,UART_HandleTypeDef* uart)
{
    for(int i = 0; i < BUFFER_SIZE; i++)
    {
        data->rx_buff[i] = 0;
    }

    data->rx_buff_ptr = data->rx_buff;
    data->index = 0;
    data->currMessageLenght = 0;
    data->expectedMessageLenght = 0;
    data->startReceive = false;
    data->endReceive = false;
    data->bufferCommand = false;
    data->rx_flag = false;
    data->receiveTimeAbort = 400;
    HAL_UART_Receive_IT (uart, data->rx_buff_ptr, 1); 
}

void NOS_UART_Timer_Handler(NOS_UART_Struct* uart)
{
        if(uart->startReceive && !uart->receiveCheck)
        {
            uart->receiveTime = 0;
            uart->receiveCheck = true;
        }

        if(uart->receiveCheck && uart->receiveTime > uart->receiveTimeAbort)
        {
            uart->receiveTime = 0;
            uart->receiveCheck = false;
            //NOS_UART_ReceiveAbort(uart,hua);
        }
        
        uart->receiveTime++;
}
/*
void NOS_UART_ReceiveHandler(NOS_UART_Struct* data,UART_HandleTypeDef* uart)
{

    data->rx_buff_ptr = &data->rx_buff[data->currMessageLenght];
    data->fuckBuff[data->fuckIndex] = *data->rx_buff_ptr;
    data->fuckIndex++;

    if((*data->rx_buff_ptr == UART_ADDRESS || *data->rx_buff_ptr == 0x66 || *data->rx_buff_ptr == 0x67 || *data->rx_buff_ptr == 0x64) && !data->startReceive)
    {
        data->startReceive = true;
        data->currMessageLenght = 0;

        data->fuckIndex = 1;
        for(int i = 0; i < BUFFER_SIZE; i++)
        {
            data->fuckBuff[i] = 0;
        }
        data->fuckBuff[0] = *data->rx_buff_ptr; 
    }

    if(data->startReceive && data->currMessageLenght == 1 && !data->bufferCommand)
    {
        data->value.bytes[1] = *data->rx_buff_ptr;
    }

    if(data->startReceive && data->currMessageLenght == 2 && !data->bufferCommand)
    {
        data->value.bytes[0] = *data->rx_buff_ptr;
        data->expectedMessageLenght = data->value.data;
        data->bufferCommand = true;
    }

    if(data->fuckIndex > BUFFER_SIZE)
    {
        data->fuckIndex = 0;
    }

    data->currMessageLenght++;

    if(data->startReceive && (data->currMessageLenght == data->expectedMessageLenght) && data->bufferCommand)
    {
        data = NOS_UART_ReceiveReset(data);
        data->bufferCommand = false;
        data->startReceive = false;
    }
    else
    {
        data = NOS_UART_ContinueReceive(data);
    }
    HAL_UART_Receive_IT (uart, data->rx_buff_ptr, 1); 
    
}
*/

uint16_t GetCRC16(uint8_t *buf, int len)
{  
  unsigned int crc = 0xFFFF;
  for (int pos = 0; pos < len; pos++)
  {
  crc ^= (unsigned int)buf[pos];    // XOR byte into least sig. byte of crc

  for (int i = 8; i != 0; i--) {    // Loop over each bit
    if ((crc & 0x0001) != 0) {      // If the LSB is set
      crc >>= 1;                    // Shift right and XOR 0xA001
      crc ^= 0xA001;
    }
    else                            // Else LSB is not set
      crc >>= 1;                    // Just shift right
    }
  }

  return crc;
}
/*
void NOS_UART_ReceiveHandler(NOS_UART_Struct* data,UART_HandleTypeDef* uart)
{
    //data->rx_buff_ptr = &data->rx_buff[data->currMessageLenght];
    data->fuckBuff[data->fuckIndex] = *data->rx_buff_ptr;
    data->fuckIndex++;

    data->startReceive = true;

    data->lastReceivedByteTime = 0; 
    data->currMessageLenght++;
    
    if(data->currMessageLenght == 32)
    {
        data->rx_buff[data->index] = *data->rx_buff_ptr;
        ++data->index;
        ++data->rx_buff_ptr;
        data->index = 0;
        data = NOS_UART_ReceiveReset(data);
    }
    else
    {
        data->rx_buff[data->index] = *data->rx_buff_ptr;
        ++data->index;
        ++data->rx_buff_ptr;
    }

    HAL_UART_Receive_IT (uart, data->rx_buff_ptr, 1); 
}
*/

void NOS_UART_ReceiveHandler(NOS_UART_Struct* data,UART_HandleTypeDef* uart)
{
    data->fuckBuff[data->fuckIndex] = *data->rx_buff_ptr;
    data->fuckIndex++;

    data->startReceive = true;

    data->lastReceivedByteTime = 0; 
    data->currMessageLenght++;
    

    data->rx_buff[data->index] = *data->rx_buff_ptr;
    ++data->index;
    ++data->rx_buff_ptr;
    

    HAL_UART_Receive_IT (uart, data->rx_buff_ptr, 1); 
}

//addr1 addr0 channel1 channel0 count1 count0 packetId3 packetId2 packetId1 packetId0 command3 command2 command1 command0 /* data (16 - 1008) */ crc16_1 crc16_0 / 
//maxLenght 1024B  minLenght 32B packetInfo 16B
    NOS_Short currCRC;
bool NOS_UART_ParsePacket(NOS_UART_Struct* data,UART_Message* message)
{
    message->address.bytes[1] = data->rx_buff[0];
    message->address.bytes[0] = data->rx_buff[1];

    message->channel.bytes[1] = data->rx_buff[2];
    message->channel.bytes[0] = data->rx_buff[3];

    message->byteCount.bytes[1] = data->rx_buff[4];
    message->byteCount.bytes[0] = data->rx_buff[5];

    message->packetId.bytes[3] = data->rx_buff[6];
    message->packetId.bytes[2] = data->rx_buff[7];
    message->packetId.bytes[1] = data->rx_buff[8];
    message->packetId.bytes[0] = data->rx_buff[9];

    message->command.bytes[3] = data->rx_buff[10];
    message->command.bytes[2] = data->rx_buff[11];
    message->command.bytes[1] = data->rx_buff[12];
    message->command.bytes[0] = data->rx_buff[13];

    for(int i = 0; i < 16; i++)
    {
        message->data[i] = data->rx_buff[14 + i];
    }

    message->CRC16.bytes[1] = data->rx_buff[30];
    message->CRC16.bytes[0] = data->rx_buff[31];

    data->value.data = GetCRC16(&data->rx_buff,30);

    if(data->value.data == message->CRC16.data)
    {
        return true;
    }

    if(message->address.data != 0)
    {
        if(message->channel.data != 0)
        {
            if(message->byteCount.data != 0)
            {
                return true;            
            }
        }
    }

}

bool NOS_UART_PacketApprovedNotice(UART_Message* message,UART_HandleTypeDef* uart)
{
    UART_Message approvalMessage;

    approvalMessage.address.data = message->address.data;
    approvalMessage.byteCount.data = message->address.data;
    approvalMessage.channel.data = message->address.data;
    approvalMessage.command.data = message->address.data;
    approvalMessage.packetId.data = message->packetId.data;
    for(int i = 0; i < 16; i++)
    {
        approvalMessage.data[i] = message->data[i];
    }

    uint8_t buff[32];
    uint8_t* ptr = &approvalMessage;

    for(int i = 0; i < 32; i++)
    {
        buff[i] = *ptr;
        ptr++;
    }

    HAL_UART_Transmit(uart,buff,32,1000);

}


bool NOS_UART_CheckReceive(NOS_UART_Struct* data)
{
    return data->rx_flag;   
}

uint8_t* NOS_UART_GetReceivedData(NOS_UART_Struct* data)
{
    return data->fuckBuff;
}

void NOS_UART_EndReceive(NOS_UART_Struct* data)
{
    data->rx_flag = false;
}
