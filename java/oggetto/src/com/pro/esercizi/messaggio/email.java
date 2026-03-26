package com.pro.esercizi.messaggio;

import com.pro.esercizi.interfaces.archiviabile;

public class email extends messaggio implements archiviabile {
  protected String oggetto;

  public email(int id, String destinatario, String testo, String oggetto) {
    super(id, destinatario, testo);
    this.oggetto = oggetto;
  }

  @Override
  public void invia() {
    System.out.println("email inviata a " + destinatario + " - Oggetto: " + oggetto);

  }

  public String getOggetto() {
    return oggetto;
  }

  public void setOggetto(String oggetto) {
    this.oggetto = oggetto;
  }

  @Override
  public String formattaPerFile() {
    return "EMAIL|" + id + "|" + destinatario + "|" + oggetto + "|" + testo;
  }

}
