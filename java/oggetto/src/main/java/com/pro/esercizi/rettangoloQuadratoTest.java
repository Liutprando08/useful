package com.pro.esercizi;

import java.util.Scanner;

import com.pro.esercizi.rettangolo;

public class rettangoloQuadratoTest {

  public static void main(String[] args) {
    Scanner in = new Scanner(System.in);
    System.out.println("inserisci base");
    double base = in.nextDouble();

    System.out.println("inserisci altezza");
    double altezza = in.nextDouble();
    rettangolo forma = new quadrato(base, altezza);
    forma.calcolaArea(base, altezza);
    forma.calcolaPerimetro(base, altezza);
    forma.toString();

  }
}
