package com.pro.esercizi;

public class tecnico extends dipendente {

  private int interventi;

  private boolean reperibili;

  public tecnico(String nome, int matricola, double pagaBase, int interventi, boolean reperibili) {
    super(nome, matricola, pagaBase);
    this.interventi = interventi;
    this.reperibili = reperibili;
  }

  public int getInterventi() {
    return interventi;
  }

  public void setReperibili(boolean reperibili) {
    this.reperibili = reperibili;
  }

  public void setInterventi(int interventi) {
    this.interventi = interventi;
  }

  public double stipendio() {
    if (reperibili == true) {

      return pagaBase + (interventi * 25) + 100;
    } else {
      return pagaBase + (interventi * 25);
    }
  }

  public boolean isReperibili() {
    return reperibili;
  }
}
