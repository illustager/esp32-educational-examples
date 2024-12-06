/*
 * This file is part of the ESP32C3 training examples.
 * 
 * Copyright (c) 2024 Yang D.L.
 * 
 * License: GPL v3
 * 
 * This example demonstrates an intentional exception by attempting 
 * to allocate an excessively large amount of memory.
 * 
 * The setup function tries to allocate memory for an array of size 1e15, 
 * which will likely cause a memory allocation failure.
 * 
 * This is used to demonstrate how the ESP32C3 handles exceptions.
 */


void setup() {
	new int[(size_t)1e15];
}

void loop() {

}
