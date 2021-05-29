void sendMessage(String outgoing, uint8_t from)
{
  //digitalWrite(LORA_SEND, HIGH);
  uint8_t data[32];
  outgoing.toCharArray((char*)data,sizeof(data));
  manager->sendtoWait(data, sizeof(data), from);
  //digitalWrite(LORA_SEND, LOW);
}
