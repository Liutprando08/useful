package com.pro.esercizi.spedizione;

import com.pro.esercizi.interfaces;

public class itinerario implements viaggio {

  protected String partenza;
  protected String destinazione;
  protected double distanzaKm;

  public String getPartenza() {
    // TODO Auto-generated method stub
    return partenza;
  }

  public String getDestinazione() {
    return destinazione;
  }

  public void setDestinazione(String destinazione) {
    this.destinazione = destinazione;
  }

  public void setPartenza(String partenza) {
    this.partenza = partenza;
  }

  public void distanzaKm(double distanzaKm) {
    this.distanzaKm = distanzaKm;
  }

  public double getDistanzaKm() {
    return distanzaKm;
  }

  @Override
  public int durataStimataGiorni() {
    // TODO Auto-generated method stub
    return (int) (distanzaKm % 500);

  }

  @Override
  public String toString() {
    return "itinerario [partenza=" + partenza + ", destinazione=" + destinazione + ", distanzaKm=" + distanzaKm
        + ", getPartenza()=" + getPartenza() + ", durataStimataGiorni()=" + durataStimataGiorni() + "]";
  }

}
