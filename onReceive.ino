bool onReceive()
{
  uint8_t buf[32];
  uint8_t len = sizeof(buf);
  uint8_t from;
  if (manager->recvfromAck(buf, &len, &from))
  {
    Serial.println((char*)buf);
    // Assume message is for me.
    digitalWrite(LORA,1); 
    //String response = "ID=,Name=Pulse,Value=" + String(accumulatedPulses) + ",Token=" + String(accumulatedDataToken) + ",";
    String response = "ID=" + node_id + ",B=" + getVoltage().substring(0,3) + ",V=" + String(accumulatedPulses) + ",T=" + String(accumulatedDataToken)  + ",";
    Serial.println(response); 
    sendMessage(response, from);
    digitalWrite(LORA,0);
  }
  return true;
}
