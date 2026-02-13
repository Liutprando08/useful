package com.pro.esercizi;

public class spedizione {
  protected String codice;
  protected double pesoKg;

  public spedizione(String codice, double pesoKg) {
    this.codice = codice;
    this.pesoKg = pesoKg;
  }

  public String getCodice() {
    return codice;
  }

  public double getPesoKg() {
    return pesoKg;
  }

  public void setCodice(String codice) {
    this.codice = codice;
  }

  public void setPesoKg(double pesoKg) {
    this.pesoKg = pesoKg;
  }

}
