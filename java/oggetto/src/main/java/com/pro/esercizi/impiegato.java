package com.pro.esercizi;

public class impiegato extends dipendente {

  private int Straordinario;

  public impiegato(String nome, int matricola, double pagaBase, int straordinario) {
    super(nome, matricola, pagaBase);
    Straordinario = straordinario;
  }

  public void setStraordinario(int straordinario) {
    Straordinario = straordinario;
  }

  public int getStraordinario() {
    return Straordinario;
  }

  @Override
  public double stipendio() {
    return pagaBase + (Straordinario * 15);
  }

}
