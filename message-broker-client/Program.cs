using System;
using System.Threading;
using Google.Protobuf.Collections;
using Grpc.Net.Client;
using System.Collections.Generic;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace MessageBrokerClient
{
    class Utils
    {
        [DllImport("Kernel32.dll")]
        private static extern IntPtr GetConsoleWindow();

        [DllImport("User32.dll")]
        private static extern IntPtr GetForegroundWindow();

        internal static bool IsConsoleInFocus() => GetConsoleWindow() == GetForegroundWindow();
    }

    class BrokerClient
    {        
        public BrokerClient(MessageBroker.MessageBrokerClient stub)
        {
            m_stub = stub;
        }

        public async Task SubscribeAsync(List<string> topics, Action<Message> onMessage, CancellationToken ct)
        {
            var protoTopics = new RepeatedField<string>();
            protoTopics.AddRange(topics);
            var subscription = m_stub.Receive(new ReceiveRequest { Topics = { protoTopics } } );
            try
            {
                while (await subscription.ResponseStream.MoveNext(ct))
                {
                    onMessage(subscription.ResponseStream.Current.Message);
                }
            }
            catch(Exception ex)
            {
                Console.WriteLine($"Got an error while getting data from the service {ex.Message}");
            }
        }

        private MessageBroker.MessageBrokerClient m_stub;
    }

    class Program
    {
        static async Task Main(string[] args)
        {
            var client = new BrokerClient(new MessageBroker.MessageBrokerClient(GrpcChannel.ForAddress("http://localhost:50051")));
            var source = new CancellationTokenSource();
            await Task.WhenAny(
                Task.Run(() => { if (Utils.IsConsoleInFocus() && Console.ReadKey().Key == ConsoleKey.Escape) source.Cancel(); }),
                client.SubscribeAsync(new List<string> { "Channel1", "Channel2" }, x => { Console.WriteLine(x.Content); }, source.Token));
            
        }
    }
}
