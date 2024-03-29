# Kobzar
A tool for creating and using simple stories in game applications. Uses the term "story" to denote a collection of interrelated textual elements. Each text scene connects to another through chains of choices and conditions that depend on the player.

Chains can consist of the following elements:
- Scene. This is a normal, non-interactive event. Just a text output, like: "You are in the main square of the city"

- Aswer. This is a description of the player's reaction, for example: "Look around". An unlimited number of Answers can be attached to each Scene.

- Script. A game script that will be executed as soon as this element is activated.

The tool is implemented as a dynamic library and has several utilities for creating and editing story files.

# Story Creator
This utility is designed to create and edit text scenes. Each scene is a separate episode of the narrative, it can contain descriptions of events, options for the player's reaction, etc. All elements of the scene are interconnected. You can create multiple scene elements and link them in the desired order. In this case, the story will branch out depending on the activation of different chains of elements. You can also add scripts to the scene. The scripts will not be visible to the player, but they will perform the necessary actions, for example, change the game parameters of the character.
