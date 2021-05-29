void countPulses()
{
  pulseCount++;
}
void displayAccumPulseCount()
{
  Serial.print("Accum Pulse Count:");
  Serial.println(accumulatedPulses);
}
void storePulseCount()
{

  accumulatedPulses = pulseCount;
  //accumulatedDataToken = now();
  accumulatedDataToken = random(10000000,99999999);
  pulseCount = 0;
  Serial.print("Storing pulses:");
  Serial.print(accumulatedPulses);
  Serial.print(",");
  Serial.println(accumulatedDataToken);
}
void checkRaining()
{
  if (pulseCount > 0)
  {
    pulseFlag = true;
    pulseFlag_s = "1";
    Serial.println("Pulse detected");
  }
  else
  {
    pulseFlag = false;
    pulseFlag_s = "0";
    Serial.println("No pulse detected");
  }
}
