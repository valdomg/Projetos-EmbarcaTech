import mongoose from "mongoose";
import dotenv from "dotenv";
import User from "../models/User.js";
import bcrypt from "bcrypt";

dotenv.config();

const seedAdmin = async () => {
  try {
    await mongoose.connect(process.env.DATABASE_URL);

    const existingAdmin = await User.findOne({ email: process.env.ADMIN_EMAIL });
    if (existingAdmin) {
      console.log("Admin já existe!");
      return;
    }

    const hashedPassword = await bcrypt.hash(process.env.ADMIN_PASSWORD, 10);

    const admin = new User({
      name: process.env.ADMIN_USERNAME,
      email: process.env.ADMIN_EMAIL,
      password: hashedPassword,
      role: "admin"
    });

    await admin.save();
    console.log("Admin criado com sucesso!");
  } catch (err) {
    console.error("Erro ao criar admin:", err);
  } finally {
    mongoose.connection.close();
  }
};

seedAdmin();