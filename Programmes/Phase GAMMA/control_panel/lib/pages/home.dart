import 'package:flutter/material.dart';
import 'dart:convert';
import 'dart:typed_data';

import 'package:control_panel/pages/side_menu.dart';
import 'package:control_panel/components/custom_button.dart';
import 'package:control_panel/components/websocket.dart';
import 'package:control_panel/constants/constants.dart';

class HomePage extends StatefulWidget {
  const HomePage({Key? key}) : super(key: key);

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  final WebSocket _socket = WebSocket(Constants.videoWebsocketURL);
  String currentText = 'Caméra désactivée';
  bool isStreaming = false;

  void toggleStreaming() {
    if (isStreaming) {
      setState(() {
        isStreaming = false;
        currentText = 'Caméra désactivée';
      });
      _socket.disconnect();
    } else {
      setState(() {
        isStreaming = true;
        currentText = 'Caméra activée';
      });
      _socket.connect();
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Panneau de contrôle'),
        backgroundColor: const Color(0xFF1331F5),
      ),
      drawer: const SideMenu(),
      body: SafeArea(
        child: Center(
          child: Column(
            children: [
              const SizedBox(height: 20),
              const Text(
                'Bienvenue dans votre espace',
                style: TextStyle(fontSize: 20),
              ),
              const Divider(),
              const SizedBox(height: 30),
              isStreaming
                  ? StreamBuilder(
                      stream: _socket.stream,
                      builder: (context, snapshot) {
                        if (!snapshot.hasData) {
                          return const CircularProgressIndicator();
                        }

                        if (snapshot.connectionState == ConnectionState.done) {
                          return const Center(
                            child: Text("Connection Closed !"),
                          );
                        }
                        return Image.memory(
                          Uint8List.fromList(
                            base64Decode(
                              (snapshot.data.toString()),
                            ),
                          ),
                          width: 640,
                          height: 480,
                          gaplessPlayback: true,
                          excludeFromSemantics: true,
                        );
                      },
                    )
                  : SizedBox(
                      width: 640,
                      height: 480,
                      child: Image.asset('img/no_img.jpg'),
                    ),
              const SizedBox(height: 50),
              ButtonSwitch(
                  onTap: toggleStreaming, text: currentText, isOn: isStreaming),
            ],
          ),
        ),
      ),
    );
  }
}
