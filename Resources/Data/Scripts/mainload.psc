if (npcTalkedTo("Neighbor")==1)
{
        print("why am i being printed?\n");
	spawnNPC("blocker2.npc",27,30,3);
}
if (npcTalkedTo("Neighbor")==0)
{
	print("No talked?\n");
	spawnNPC("blocker1.npc",26,33,0);
}