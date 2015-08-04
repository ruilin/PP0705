package com.ryangame.pet.gl;



public class Platform {
	
	public int state;
	public InputEvent[] inputs;
	public boolean isInited;
	
	public Platform() {
		state = 0;
		isInited = false;
	}
	
	public void reset() {
		isInited = false;
	}
}
