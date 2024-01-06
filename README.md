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
The controls are simple. Utilize the WASD keys for movement. When the moment calls for it, unleash attacks with the Left Mouse Button.

As the clock ticks down, you'll need to outmaneuver your rivals. Every swing of your sword, and every well-timed strike brings you closer to victory.

![image](https://github.com/JwL-01/SwordGame/assets/38309953/80744be8-b030-47db-b56a-2ad02f32088a)

Using Unreal Engine 4.27 and enhanced with marketplace assets.
