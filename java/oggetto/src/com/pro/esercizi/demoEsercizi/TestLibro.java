package com.pro.esercizi.demoEsercizi;

import java.util.ArrayList;

import com.pro.esercizi.biblioteca.*;

public class TestLibro {
  public static void main(String[] args) {
    ArrayList<libro> arr = new ArrayList<>();
    romanzo rom = new romanzo("pino", "giggi", 12, "maschio");
    fumetto fum = new fumetto("cacamin", "adad", 23, "dimaio");
    arr.add(rom);
    arr.add(fum);
  }

}
