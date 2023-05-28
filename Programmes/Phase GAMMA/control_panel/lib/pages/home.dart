import 'package:flutter/material.dart';
import 'package:control_panel/pages/side_menu.dart';

class HomePage extends StatefulWidget {
  const HomePage({super.key});

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Control Panel Travelers'),
        backgroundColor: const Color(0xFF1331F5),
      ),
      drawer: const SideMenu(),
    );
  }
}
