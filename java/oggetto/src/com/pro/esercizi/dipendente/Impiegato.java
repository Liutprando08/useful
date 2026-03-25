public class Impiegato extends Dipendente {

    public Impiegato(String nome, String cognome, double stipendioBaseMensile) {
        super(nome, cognome, stipendioBaseMensile);
    }

    @Override
    public double calcolaStipendioAnnuo() {
        return getStipendioBaseMensile() * 12;
    }

    @Override
    public String toString() {
        return "Impiegato{" +
                "nome='" + getNome() + '\'' +
                ", cognome='" + getCognome() + '\'' +
                ", stipendioBaseMensile=" + getStipendioBaseMensile() +
                ", stipendioAnnuo=" + calcolaStipendioAnnuo() +
                '}';
    }
}
