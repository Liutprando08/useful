package com.pro.esercizi;

public class frazione {

  private int num1;
  private int num2;

  public frazione(int num1, int num2) {
    this.num1 = num1;
    this.num2 = num2;
  }

  public int getNum1() {
    return num1;
  }

  public int getNum2() {
    return num2;
  }

  public void setNum1(int num1) {
    this.num1 = num1;
  }

  public void setNum2(int num2) {
    this.num2 = num2;
  }

  public int somma() {
    return num1 + num2;
  }

  public int sottrazzione() {
    return num1 - num2;
  }

  public int moltiplicazione() {
    return num1 * num2;
  }

  public int devisione() {
    return num1 / num2;
  }

  public boolean equals() {
    return num1 == num2;
  }

  public boolean isMinore() {
    return num1 < num2;
  }

  public String toString() {
    return "num1 " + num1 + " num2 " + num2 + " minore " + isMinore() + " moltoplicazione " + moltiplicazione();
  }

}
