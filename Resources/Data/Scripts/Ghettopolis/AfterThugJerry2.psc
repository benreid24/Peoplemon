//---Battle Won, trainer still won't move until Marsala makes him move
sleep(1000);
messageBox("..so..");
removeNPC("Marsala2");
spawnNPC("Ghettopolis/Marsala2.npc",414,172,0,1);
sleep(1000);
npcToLocation("Marsala2",400,175,1);
moveNPC("Marsala2",0);
messageBox("Hey, Thug dude!", "Move!");
sleep(300);
messageBox("*gulp*","The..", "the boss.. he..", "..he said..");
sleep(300);
messageBox("Let's get to the meat and potatos here.", "I'm bigger and I live off a steady diet of liquid protein powder..");
sleep(700);
messageBox("You can pass, just don't hurt me");
sleep(200);
trainerToLocation("ThugJerry2",399,172,1);
moveTrainer("ThugJerry2",1);
sleep(1000);
moveNPC("Marsala2",0);
sleep(200);
messageBox("Okay let's go take care of this chump");
sleep(1000);

removeNPC("ThugJaq");
removeNPC("ThugJacob");
removeNPC("ThugJimbo");
removeNPC("ThugJin");
removeNPC("Thug Jimmy");
removeNPC("ThugJil");
removeNPC("ThugJay");
removeNPC("ThugJojo");
sleep(20); //wait for removal que to be emptied before spawning and moving npcs

spawnNPC("Ghettopolis/ThugJaq.npc",400,183,1);
spawnNPC("Ghettopolis/ThugJacob.npc",401,183,1);
spawnNPC("Ghettopolis/ThugJimbo.npc",400,184,1);
spawnNPC("Ghettopolis/ThugJin.npc",401,185,1);
spawnNPC("Ghettopolis/ThugJoe.npc",396,187,1);
spawnNPC("Ghettopolis/ThugJimmy.npc",416,170,1);
spawnNPC("Ghettopolis/ThugJil.npc",416,169,1);
spawnNPC("Ghettopolis/ThugJay.npc",395,187,1);
spawnNPC("Ghettopolis/ThugJojo.npc",416,168,1);
messageBox("Hey! Stop!");

runScript("npcToLocation(\"ThugJaq\",399,176,1);moveNPC(\"ThugJaq\",0);",1);
runScript("npcToLocation(\"ThugJacob\",400,177,1);moveNPC(\"ThugJacob\",0);",1);
runScript("npcToLocation(\"ThugJimbo\",401,177,1);moveNPC(\"ThugJimbo\",0);",1);
runScript("npcToLocation(\"ThugJin\",400,178,1);moveNPC(\"ThugJin\",0);",1);
runScript("npcToLocation(\"ThugJoe\",403,177,1);moveNPC(\"ThugJoe\",0);",1);
runScript("npcToLocation(\"ThugJil\",402,178,1);moveNPC(\"ThugJil\",0);",1);
runScript("npcToLocation(\"ThugJay\",399,179,1);moveNPC(\"ThugJay\",0);",1);
runScript("npcToLocation(\"ThugJojo\",400,180,1);moveNPC(\"ThugJojo\",0);",1);
sleep(1000);
runScript("npcToLocation(\"Thug Jimmy\",401,179,1);moveNPC(\"Thug Jimmy\",0);",1);

sleep(4000);
moveNPC("Marsala2",2);
sleep(1000);	
moveNPC("Marsala2",1);
sleep(1000);
moveNPC("Marsala2",2);
sleep(1500);
moveNPC("Marsala2",0);
sleep(2000);
messageBox("I can take these chumps kid","Just go in there and show their boss who's boss");
sleep(2500);
moveNPC("Marsala2", 2);
sleep(1000);
print("moving you");
addSaveEntry("ReadyForBoss1",1);
playerToLocation(401,171,1);
print("done?");