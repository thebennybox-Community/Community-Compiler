namespace ilc.Nasm
{
    public class Or
    {
        public Registers Destination { get; set; }
        public Registers Source { get; set; }

        public Or(Registers destination, Registers source)
        {
            Destination = destination;
            Source = source;
        }

        public override string ToString()
        {
            return $"or {Destination}, {Source}";
        }
    }
}