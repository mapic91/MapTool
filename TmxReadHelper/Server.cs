using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using Google.Protobuf;

namespace TmxReadHelper
{

    public class StateObject
    {
        // Client  socket.
        public Socket workSocket = null;
        // Size of receive buffer.
        public const int BufferSize = 1024;
        // Receive buffer.
        public byte[] buffer = new byte[BufferSize];
        public byte[] data;
        public int dataReceived;
    }

    public class Server
    {
        public const byte ExitServerCode = 0;
        public const byte OpenMapCode = 1;
        public const byte AnoderServerRun = 2;

        public const byte ResultOk = 0;
        public const byte ResultFailed = 1;

        private int _listenPort;
        private bool _exit;
        private int _serverRuned = 1;
        private ManualResetEvent allDone = new ManualResetEvent(false);
        public Server(int port)
        {
            _listenPort = port;
        }

        public void Run()
        {
            RunInternal();
        }

        public bool CheckIsRunning()
        {
            try
            {
                Socket client = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                client.Connect(new IPEndPoint(0x0100007F, _listenPort));
                client.Send(new byte[] {0x01, 0x00, 0x00, 0x00, AnoderServerRun});
                client.Receive(new byte[1]); // Wait echo back
            }
            catch (SocketException e)
            {
                return false;
            }
            return true;
        }

        private bool RunInternal()
        {
            Socket listener = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                listener.Bind(new IPEndPoint(0x0100007F,_listenPort));
                listener.Listen(100);

                while (true)
                {
                    if (_exit)
                    {
                        listener.Close();
                        break;
                    }
                    allDone.Reset();
                    listener.BeginAccept(AcceptCallback, listener);
                    allDone.WaitOne();
                }
            }
            catch (Exception e)
            {
                return false;
            }
            return true;
        }

        private void AcceptCallback(IAsyncResult ar)
        {
            try
            {
                if (_exit) return;
                allDone.Set();
                Socket listener = (Socket)ar.AsyncState;
                Socket handler = listener.EndAccept(ar);

                StateObject state = new StateObject();
                state.workSocket = handler;
                handler.BeginReceive(state.buffer, 0, StateObject.BufferSize, 0, ReadCallback, state);
            }
            catch (SocketException)
            {

            }
        }

        private void ReadCallback(IAsyncResult ar)
        {
            try
            {
                StateObject state = (StateObject)ar.AsyncState;
                Socket handler = state.workSocket;
                int bytesRead = handler.EndReceive(ar);
                if (state.data == null)
                {
                    if (bytesRead >= 4)
                    {
                        int startIndex = 0;
                        var dataLen = GetLittleEndianIntegerFromByteArray(state.buffer, ref startIndex);
                        state.data = new byte[dataLen];
                        state.dataReceived = bytesRead - 4;
                        if (bytesRead > 4)
                        {
                            Array.Copy(state.buffer, 4, state.data, 0, bytesRead - 4);
                        }
                    }
                }
                else
                {
                    Array.Copy(state.buffer, 0, state.data, state.dataReceived, bytesRead);
                }

                if (state.data == null)
                {
                    handler.BeginReceive(state.buffer, bytesRead, StateObject.BufferSize, 0, ReadCallback, state);
                }
                else if (state.dataReceived == state.data.Length)
                {
                    //All data received
                    var data = state.data;
                    switch (data[0])
                    {
                        case ExitServerCode:
                            _serverRuned--;
                            handler.Shutdown(SocketShutdown.Both);
                            handler.Close();
                            if (_serverRuned == 0)
                            {
                                _exit = true;
                                allDone.Set();
                            }
                            break;
                        case OpenMapCode:
                            var path = Encoding.UTF8.GetString(data, 1, data.Length - 1);
                            var tmxMap = TmxReader.ReadTmxMap(path);
                            if (tmxMap == null)
                            {
                                Send(handler, ResultFailed, null);
                            }
                            else
                            {
                                Send(handler, ResultOk, tmxMap.ToByteArray());
                            }
                            break;
                        case AnoderServerRun:
                            Send(handler, 0, null);
                            _serverRuned++;
                            break;
                    }
                }
                else
                {
                    //Receive remain data
                    handler.BeginReceive(state.buffer, 0, StateObject.BufferSize, 0, ReadCallback, state);
                }
            }
            catch (SocketException e)
            {

            }
        }

        private void Send(Socket handler, byte result, byte[] data)
        {
            try
            {
                var dataLen = data == null ? 0 : data.Length;
                var buffer = new byte[dataLen + 5];
                ToLittleEndian(dataLen + 1, ref buffer);
                buffer[4] = result;
                if (data != null)
                {
                    Array.Copy(data, 0, buffer, 5, data.Length);
                }

                handler.BeginSend(buffer, 0, buffer.Length, 0, SendCallback, handler);
            }
            catch (SocketException e)
            {

            }
        }

        private void SendCallback(IAsyncResult ar)
        {
            try
            {
                // Retrieve the socket from the state object.
                Socket handler = (Socket)ar.AsyncState;

                // Complete sending the data to the remote device.
                int bytesSent = handler.EndSend(ar);

                handler.Shutdown(SocketShutdown.Both);
                handler.Close();
            }
            catch (SocketException e)
            {
                
            }
        }

        static public int GetLittleEndianIntegerFromByteArray(byte[] data, ref int startIndex)
        {
            var ret = (data[startIndex + 3] << 24)
                 | (data[startIndex + 2] << 16)
                 | (data[startIndex + 1] << 8)
                 | data[startIndex];
            startIndex += 4;
            return ret;
        }

        static public void ToLittleEndian(int value, ref byte[] outBytes, int startIndex = 0)
        {
            outBytes[startIndex] = (byte)(value & 0xFF);
            outBytes[startIndex+1] = (byte)((value >> 8) & 0xFF);
            outBytes[startIndex+2] = (byte)((value >> 16) & 0xFF);
            outBytes[startIndex+3] = (byte)((value >> 24) & 0xFF);
        }
    }
}