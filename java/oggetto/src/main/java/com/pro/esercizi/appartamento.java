package com.pro.esercizi;

import java.awt.geom.Area;

public class appartamento {
  private String name;
  private double area;
  private int inquilini;
  private int piani;

  public appartamento(String name, double area, int inquilini, int piani) {
    this.name = name;
    this.area = area;
    this.inquilini = inquilini;
    this.piani = piani;
  }

  public void setName(String name) {
    this.name = name;
  }

  public void setInquilini(int inquilini) {
    this.inquilini = inquilini;
  }

  public double getArea() {
    return area;
  }

  public int getInquilini() {
    return inquilini;
  }

  public String getName() {
    return name;
  }

  public int getPiani() {
    return piani;
  }

  public String toString() {
    return "Appartamento {" +
        "Proprietario='" + name + '\'' +
        ", Superficie=" + area + " mq" +
        ", Piano=" + piani +
        ", Inquilini=" + inquilini +
        '}';
  }

}
