if(getSaveEntry("WellThugsBeat")!=1)
{
 spawnTrainer("TheWell/ThugPaul.tnr",16,39,2,0);
 spawnTrainer("TheWell/ThugMary.tnr",18,18,0,0);
 if (trainerDefeated("ThugPeter")!=1)
  {
   spawnTrainer("TheWell/ThugPeter.tnr",32,5,2,0);
  }
 else
  spawnTrainer("TheWell/ThugPeter.tnr",32,7,2,0);
}

if(getSaveEntry("BeatIce")==1 && getSaveEntry("VincingtonBeat")!=1) {
 spawnTrainer("TheWell/InvThug1.tnr",6,40,1,0);
 spawnTrainer("TheWell/InvThug2.tnr",16,41,0,0);
 spawnTrainer("TheWell/InvThug6.tnr",3,8,2,0);
 spawnTrainer("TheWell/InvThug3.tnr",25,27,2,0);
 spawnTrainer("TheWell/InvThug4.tnr",34,7,1,0);
 spawnTrainer("TheWell/InvThug5.tnr",44,25,2,0);
 spawnTrainer("TheWell/InvThug5.tnr",42,26,2,0);
 spawnTrainer("TheWell/InvThug5.tnr",40,27,2,0);
}