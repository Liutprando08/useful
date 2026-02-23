package com.pro.esercizi;

public class UfficioPersonale {
  private impiegato impiegato;
  private tecnico tecnico;

  public UfficioPersonale(impiegato impiegato, tecnico tecnico) {
    this.impiegato = impiegato;
    this.tecnico = tecnico;
  }

  public impiegato getImpiegato() {
    return impiegato;
  }

  public void setImpiegato(impiegato impiegato) {
    this.impiegato = impiegato;
  }

  public tecnico getTecnico() {
    return tecnico;
  }

  public void setTecnico(tecnico tecnico) {
    this.tecnico = tecnico;
  }

  public double totaleStipendi() {
    return impiegato.stipendio() + tecnico.stipendio();
  }

  public double PagaBaseImpiegato(double aumento) {
    return impiegato.stipendio() + aumento;
  }

  public double PagaBaseTecnico(double aumento) {
    return tecnico.stipendio() + aumento;
  }
}
