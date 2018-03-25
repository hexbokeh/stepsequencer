void passArray(boolean arr[][4])
{
  Serial.print(arr[currentChannel][currentStep]);
  Serial.print(" ");
  if (currentStep == 3) Serial.println();
  //  Serial.println();
}
