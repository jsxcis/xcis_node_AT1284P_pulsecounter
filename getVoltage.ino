String getVoltage()
{
    float batteryVoltage = 0;
    batteryVoltage = (analogRead(BATTERY_VOLTAGE) * 0.006325);
    return String(batteryVoltage);
}
