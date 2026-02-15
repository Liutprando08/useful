package com.pro.esercizi;

public class spedizione {
  protected String codice;
  protected double pesoKg;
  protected String zona;
  protected boolean sabato;

  public spedizione(String codice, double pesoKg) {
    this.codice = codice;
    this.pesoKg = pesoKg;
  }

  public void setZona(String zona) {
    this.zona = zona;
  }

  public void setSabato(boolean sabato) {
    this.sabato = sabato;
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

  public double costo() {
    return 5 + 1.2 * pesoKg;
  }

  public String toString() {
    return "il codice e " + costo();
  }

}
