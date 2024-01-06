**Introduction**
---
In this game, each round lasts 60 seconds, during which you must harness your combat skills to their fullest. Armed with your trusty sword and dodging abilities, you'll engage in a showdown against other players, all for the ultimate duel of skill and strategy.

**Project Goals & Objectives**
---
The purpose of this project was to learn how to deal with networking and multiplayer in Unreal Engine 4. I experimented with replication, the process of synchronizing data and procedure calls between clients and servers. Some key things that I learned:
- RPCs (Remote Procedure Calls) were used to send movement, and attacking for the sword character to other players in the game. They are used when you want one instance of the game to call a function or execute code on another instance of the game running elsewhere. Within RPCs you specify the type of RPC function for either the client, server or multicast. 
- The Actor Role and Remote Role:
  - The Actor Role refers to the role an actor assumes in the context of network replication and gameplay. Role_Authority indicates that the actor exists and is controlled by the server. It has authority over its state and replication to clients. Role_SimulatedProxy denotes that the actor exists on a client and is being simulated to replicate the behaviour of the authority server version. Role_AutonomousProxy is when the actor is controlled locally by a player and is responsible for handling its simulation on the client side.
  - The Remote Role refers to how a particular actor or object is perceived on a remote client in a networked environment. Role SimulatedProxy indicates the actor is remotely controlled proxy that is simulated on the client to replicate the behaviour of the actor from the server. Role_AutonomousProxy denotes that the actor is controlled by the local player and should be autonomously simulated on the client.

**Controls & Gameplay**
---
The controls are intuitive, ensuring a seamless experience for players of all levels. Utilize the classic WASD keys for movement, swiftly maneuvering around the battleground to gain the upper hand. When the moment calls for it, unleash devastating attacks with the Left Mouse Button, each strike bringing you closer to victory and your opponents closer to defeat.

Survival is paramount in this intense arena. As the clock ticks down, you'll need to outwit, outmaneuver, and outlast your rivals. Every swing of your sword, every evasive roll, and every well-timed strike brings you closer to victory. The thrill of narrowly escaping an opponent's blade or landing a perfectly timed counterattack will keep your heart racing throughout each round.

With each round, tension builds as players clash in a symphony of steel and fury. The objective is simple: be the last warrior standing. Whether you're cunningly dodging incoming blows or aggressively pursuing your adversaries, adaptability and quick thinking are your greatest allies in this electrifying contest.

But the battle doesn't end with a single victory. Rounds will continue to pulsate with excitement as they restart in 60 seconds or immediately when a single player emerges victorious, leaving little time for respite. The cycle of combat begins anew, giving players the chance to learn, adapt, and strive for victory once more.

![image](https://github.com/JwL-01/SwordGame/assets/38309953/80744be8-b030-47db-b56a-2ad02f32088a)

Brought to life with the power of Unreal Engine 4.27 and enhanced with marketplace assets, my multiplayer game offers a captivating blend of skill-based gameplay and heart-pounding action. Are you ready to prove your mettle in the arena, carving your path to glory one swing of the sword at a time? The chaos awaits, and only the strongest will emerge as the ultimate champion!
