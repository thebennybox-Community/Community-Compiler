namespace ilc.Nasm
{
    public class Div
    {
        public Registers Destination { get; set; }
        public Registers Source { get; set; }

        public Div(Registers destination, Registers source)
        {
            Destination = destination;
            Source = source;
        }

        public override string ToString()
        {
            return $"div {Destination}, {Source}";
        }
    }
}